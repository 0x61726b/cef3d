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

using namespace Cef3D;

#pragma warning(disable:4611)
#pragma warning(disable:4267)

bool GIsExiting = false;

typedef unsigned char ui8;
#define ASSERT_EX(cond, error_message) do { if (!(cond)) { std::cerr << error_message; exit(1);} } while(0)

struct TPngDestructor {
	png_struct *p;
	TPngDestructor(png_struct *p) : p(p) {}
	~TPngDestructor() { if (p) { png_destroy_write_struct(&p, NULL); } }
};

static void PngWriteCallback(png_structp  png_ptr, png_bytep data, png_size_t length) {
	std::vector<ui8> *p = (std::vector<ui8>*)png_get_io_ptr(png_ptr);
	p->insert(p->end(), data, data + length);
}

void WritePngToMemory(size_t w, size_t h, const ui8 *dataRGBA, std::vector<ui8> *out) {
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
		std::cout << "Rendering frame #" << frameCount << std::endl;
		std::vector<unsigned char> out;
		WritePngToMemory(width, height, (unsigned char*)buffer, &out);

		std::ofstream outfile("D:\\Arken\\C++\\cef3d\\Cef3D\\Binaries\\Win64\\test.png", std::ios::out | std::ios::binary);
		outfile.write((const char*)out.data(), out.size());

		frameCount++;

		if (isLoadingComplete && !GIsExiting)
			GIsExiting = true;
	}

	virtual void OnSetLoadingState(bool isLoading, bool canGoBack, bool canGoForward)
	{
		isLoadingComplete = !isLoading;
		std::cout << "Loading state:" << isLoading << std::endl;
		if (isLoadingComplete && frameCount >= 1)
			GIsExiting = true;
		//Invalidate();

		/*if(!isLoading)
			GIsExiting = true;*/
	}

	virtual void OnDestroyed()
	{

	}

private:
	bool isLoadingComplete;
	bool isClosing;
	int frameCount;
};

int main(int* argc, char** argv)
{
	auto start = std::chrono::steady_clock::now();

	bool isSubProcessed = true;

	Cef3D::Cef3DDefinition definition;
	definition.UseChildProcess = isSubProcessed;
	definition.OffscreenRendering = true;
	definition.UseCefLoop = false;

	bool init = Cef3D_Init(definition);

	if (!init)
		return -1;

	Cef3D::Cef3DBrowserDefinition def;
	def.DefaultUrl = "D:\\Arken\\C++\\cef3d\\Demos\\OSR_SavetoFile\\index2.html";
	def.Rect = Cef3D::Cef3DRect(600, 400);

	std::auto_ptr<SampleBrowser> browser2;
	browser2.reset(new SampleBrowser());
	if (!Cef3D_CreateBrowser(browser2.get(), def))
		return -1;

	while (!GIsExiting)
	{
		Cef3D_PumpMessageLoop();
	}

	browser2->Close(true);

	Cef3D_Shutdown();

	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	std::cout << "Time of exectuion: " << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;

	return 0;
}