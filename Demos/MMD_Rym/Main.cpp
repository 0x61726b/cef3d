//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.cpp
// Date: 19.04.2017
//---------------------------------------------------------------------------

#include <Cef3D.h>
#include <Cef3DPCH.h>
#include <png.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <chrono>
#include <time.h>

using namespace Cef3D;

#pragma warning(disable:4611)
#pragma warning(disable:4267)


// Indicate that we want to exit the main loop
bool GIsExiting = false;

// HTML file we want to render in absolute path
char* SourceHTMLTargetPath;

// Absolute path of the PNG file that will be created
char* TargetPngPath;

// Exit after frame threshold
int MaxFrameCountBeforeExit = 4;


// Current frame count
int RenderedFrameCount = 0;
int OnPaintFrameCount = 0;

int TargetWidth = -1;
int TargetHeight = -1;

typedef unsigned char ui8;
#define ASSERT_EX(cond, error_message) do { if (!(cond)) { std::cerr << error_message; exit(1);} } while(0)

struct TPngDestructor
{
	png_struct *p;
	TPngDestructor(png_struct *p) : p(p) {}
	~TPngDestructor() { if (p) { png_destroy_write_struct(&p, NULL); } }
};

static void PngWriteCallback(png_structp  png_ptr, png_bytep data, png_size_t length)
{
	std::vector<ui8> *p = (std::vector<ui8>*)png_get_io_ptr(png_ptr);
	p->insert(p->end(), data, data + length);
}

void WritePngToMemory(size_t w, size_t h, const ui8 *dataRGBA, std::vector<ui8> *out)
{
	out->clear();
	png_structp p = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	ASSERT_EX(p, "png_create_write_struct() failed");
	TPngDestructor destroyPng(p);
	png_infop info_ptr = png_create_info_struct(p);

	ASSERT_EX(info_ptr, "png_create_info_struct() failed");
	ASSERT_EX(0 == setjmp(png_jmpbuf(p)), "setjmp(png_jmpbuf(p) failed");
	png_set_IHDR(p, info_ptr, w, h, 8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
	png_set_bgr(p);
	//png_set_compression_level(p, 1);
	std::vector<ui8*> rows(h);
	for (size_t y = 0; y < h; ++y)
		rows[y] = (ui8*)dataRGBA + y * w * 4;
	png_set_rows(p, info_ptr, &rows[0]);
	png_set_write_fn(p, out, PngWriteCallback, NULL);
	png_write_png(p, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
}

/*
* Html Browser instance that will be used to render |SourceHTMLTargetPath|
* Deriving from |Cef3DBrowser| class lets us override the OnPaint method which will be called by Chromium with the invalidated pixel buffer.
*/

#include <algorithm>
#include <iterator>
#include <set>



class SampleBrowser : public Cef3DBrowser
{
public:
	void OnPaint(
		Cef3D::Cef3DOsrRenderType type,
		const std::vector<Cef3D::Cef3DRect>& dirtyRects,
		const void* buffer,
		int width,
		int height) override
	{
		std::cout << "OnPaint frame # " << OnPaintFrameCount << std::endl;

		if (OnPaintFrameCount >= MaxFrameCountBeforeExit)
			Render(buffer);

		/*if (RenderedFrameCount >= MaxFrameCountBeforeExit && exitReceived)
		GIsExiting = true;*/
		OnPaintFrameCount++;
	}

	void Render(const void* buffer)
	{
		std::cout << "Rendering frame #" << RenderedFrameCount << std::endl;

		std::vector<unsigned char> out;
		WritePngToMemory(TargetWidth, TargetHeight, (unsigned char*)buffer, &out);

		std::ofstream outfile(TargetPngPath, std::ios::out | std::ios::binary);

		if (outfile.is_open())
		{
			outfile.write((const char*)out.data(), out.size());

			if (outfile.bad())
			{
				std::cout << "Could not save!";
				return;
			}
		}
		else
		{
			std::cout << "Could not open the file !";
			return;
		}

		first_frame_rendered = true;
		RenderedFrameCount++;

		/*if (first_frame_rendered)
			GIsExiting = true;*/
	}

	virtual void OnProcessMessageReceived(const std::string& name)
	{
		if (name == "exitApp")
		{
			exitReceived = true;
			/*if(RenderedFrameCount >= MaxFrameCountBeforeExit)
			GIsExiting = true;*/
		}
	}

	virtual void OnAfterCreated()
	{
		
	}

private:
	bool first_frame_rendered;
	bool exitReceived;
};




class AppDelegate : public Cef3D::Cef3DAppDelegate
{
public:
	virtual void OnBeforeChildProcessLaunch(Cef3D::Cef3DCommandLine& CmdLine) override
	{

	}
};

int main(int argc, char** argv)
{
	auto start = std::chrono::steady_clock::now();

	/* Initialize Cef3D */
	bool isSubProcessed = true;

	Cef3D::Cef3DDefinition definition;
	definition.UseChildProcess = isSubProcessed;
	definition.OffscreenRendering = true;
	definition.UseCefLoop = false;
	definition.LogLevel = Cef3DLogLevel::Warning;

	AppDelegate appDel;
	bool init = Cef3D_Init(definition, &appDel);

	if (!init)
		return -1;

	Cef3D::Cef3DBrowserDefinition def;
	def.DefaultUrl = SourceHTMLTargetPath;
	def.LoadImmediately = true;
	def.Rect = Cef3D::Cef3DRect(710, 300 + 170);

	TargetWidth = 710;
	TargetHeight = 300;

	std::auto_ptr<SampleBrowser> browser2;
	browser2.reset(new SampleBrowser());
	if (!Cef3D_CreateBrowser(browser2.get(), def))
		return -1;

	while (!GIsExiting)
	{
		Cef3D_PumpMessageLoop();
	}

	std::cout << "Shutting down.." << std::endl;

	/* We have exited the main loop, means we are exiting. Close the browser*/
	browser2->Close(true);


	/* Shutdown */
	Cef3D_Shutdown();

	/* Measure elapsed time */
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	std::cout << "Time of exectuion: " << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;

	return 0;
}