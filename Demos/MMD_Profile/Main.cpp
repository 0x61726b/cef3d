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


// Current frame count
int RenderedFrameCount = 0;
int OnPaintFrameCount = 0;

int TargetWidth = -1;
int TargetHeight = -1;

struct ImageEntry
{
	char* Cover;
	char* Text;
};

struct UserInfo
{
	char* Name;
	char* ScrobbleCount;
	char* ArtistCount;
	char* AlbumCount;
	char* Avatar;
};

ImageEntry topAlbum;
ImageEntry topArtist;
int topArtistCount = 0;
int topAlbumCount = 0;
std::vector<ImageEntry> topAlbums;
std::vector<ImageEntry> topArtists;
UserInfo userInfo;
char* tags;

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

		if (first_frame_rendered)
			GIsExiting = true;
	}

	virtual void OnProcessMessageReceived(const std::string& name)
	{

	}

	virtual void OnAfterCreated()
	{
		Cef3DJsValue jsTopAlbumCount("cppAlbumCount", std::to_string(topAlbumCount));
		Cef3DJsValue jsTopArtistCount("cppArtistCount", std::to_string(topArtistCount));

		Cef3DJsValue jsTopAlbumText("cppTopAlbumText", topAlbum.Text);
		Cef3DJsValue jsTopAlbumCover("cppTopAlbumCover", topAlbum.Cover);

		Cef3DJsValue jsTopArtistText("cppTopArtistText", topArtist.Text);
		Cef3DJsValue jsTopArtistCover("cppTopArtistCover", topArtist.Cover);

		Cef3DJsValue jsUserArtists("cppUserArtists", userInfo.ArtistCount);
		Cef3DJsValue jsUserAlbums("cppUserAlbums", userInfo.AlbumCount);
		Cef3DJsValue jsUserScrobbles("cppUserScrobbles", userInfo.ScrobbleCount);
		Cef3DJsValue jsUserAvatar("cppUserAvatar", userInfo.Avatar);
		Cef3DJsValue jsUserName("cppUserName", userInfo.Name);

		Cef3DJsValue jsTags("cppTags", tags);

		CreateJsObject(jsTopAlbumCount);
		CreateJsObject(jsTopArtistCount);

		CreateJsObject(jsTopAlbumText);
		CreateJsObject(jsTopAlbumCover);

		CreateJsObject(jsTopArtistText);
		CreateJsObject(jsTopArtistCover);

		CreateJsObject(jsUserArtists);
		CreateJsObject(jsUserAlbums);
		CreateJsObject(jsUserScrobbles);
		CreateJsObject(jsUserAvatar);

		CreateJsObject(jsTags);
		CreateJsObject(jsUserName);

		for (int i = 0; i < topAlbumCount; i++)
		{
			Cef3DJsValue jsTopAlbumText_("cppTopAlbum_" + std::to_string(i) + "_Text", topAlbums[i].Text);
			Cef3DJsValue jsTopAlbumCover_("cppTopAlbum_" + std::to_string(i) + "_Cover", topAlbums[i].Cover);

			CreateJsObject(jsTopAlbumText_);
			CreateJsObject(jsTopAlbumCover_);
		}

		for (int i = 0; i < topArtistCount; i++)
		{
			Cef3DJsValue jsTopArtistText_("cppTopArtist_" + std::to_string(i) + "_Text", topArtists[i].Text);
			Cef3DJsValue jsTopArtistCover_("cppTopArtist_" + std::to_string(i) + "_Cover", topArtists[i].Cover);

			CreateJsObject(jsTopArtistText_);
			CreateJsObject(jsTopArtistCover_);
		}

		
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

	if (argc <= 5)
		return -1;
	
	// Read arguments from command line
	SourceHTMLTargetPath = argv[1];
	TargetPngPath = argv[2];

	topAlbumCount = atoi(argv[4]);
	topArtistCount = atoi(argv[3]);

	int expected_argc = 1 + 14 + topAlbumCount * 2 + topArtistCount * 2;

	if (argc != expected_argc)
	{
		std::cout << "unpexpected exit. Arg count: " << argc << " expected:" << expected_argc;
		return -1;
	}


	topArtist.Text = argv[5];
	topArtist.Cover = argv[6];

	topAlbum.Text = argv[7];
	topAlbum.Cover = argv[8];

	userInfo.Name = argv[9];
	userInfo.ArtistCount = argv[10];
	userInfo.AlbumCount = argv[11];
	userInfo.ScrobbleCount = argv[12];
	userInfo.Avatar = argv[13];
	tags = argv[14];

	int j = 0;
	for (int i = 0; i < topArtistCount*2; i+=2)
	{
		ImageEntry top_artist;
		top_artist.Text = argv[15 + i];
		top_artist.Cover = argv[15 + i + 1];
		topArtists.push_back(top_artist);
		j++;
	}

	j = 0;
	for (int i = 0; i < topAlbumCount * 2; i += 2)
	{
		ImageEntry top_album;
		top_album.Text = argv[15 + topArtistCount*2 + i];
		top_album.Cover = argv[15 + topArtistCount*2 +  i + 1];
		topAlbums.push_back(top_album);
		j++;
	}


	

	// Output the arguments
	std::cout << "---------------Cef3D-------------" << std::endl;
	std::cout << "Target URL:" << SourceHTMLTargetPath << std::endl;
	std::cout << "Target PNG:" << TargetPngPath << std::endl;

	std::cout << "Top Artist Count:" << topArtistCount << std::endl;
	std::cout << "Top Album Count:" << topAlbumCount << std::endl;

	std::cout << "Top Artist text:" << topArtist.Text << std::endl;
	std::cout << "Top Artist cover:" << topArtist.Cover << std::endl;

	std::cout << "Top Album text:" << topAlbum.Text << std::endl;
	std::cout << "Top Album cover:" << topAlbum.Cover << std::endl;

	std::cout << "UserInfo name:" << userInfo.Name << std::endl;
	std::cout << "UserInfo artist count:" << userInfo.ArtistCount << std::endl;
	std::cout << "UserInfo album count:" << userInfo.AlbumCount << std::endl;
	std::cout << "UserInfo scrobble count:" << userInfo.ScrobbleCount << std::endl;

	for (int i = 0; i < topArtistCount; i++)
	{
		std::cout << "Top Artist " << std::to_string(i) << " text:" << topArtists[i].Text << std::endl;
		std::cout << "Top Artist " << std::to_string(i) << " cover:" << topArtists[i].Cover << std::endl;
	}

	for (int i = 0; i < topAlbumCount; i++)
	{
		std::cout << "Top Album " << std::to_string(i) << " text:" << topAlbums[i].Text << std::endl;
		std::cout << "Top Album " << std::to_string(i) << " cover:" << topAlbums[i].Cover << std::endl;
	}
	

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
	def.Rect = Cef3D::Cef3DRect(1200, 970 + 170);
	

	TargetWidth = 1200;
	TargetHeight = 970;

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