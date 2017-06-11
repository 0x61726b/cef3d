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
int MaxFrameCountBeforeExit = 15;


// Current frame count
int RenderedFrameCount = 0;
int OnPaintFrameCount = 0;

int TargetWidth = -1;
int TargetHeight = -1;


struct PreviousTrack
{
	char* Cover;
	char* Text;
};

struct LastfmScrobbleEntry
{
	char* SongName;
	char* ArtistName;
	char* AlbumCover;
	char* ArtistCover;
	char* ArtistScrobbles;
	char* AlbumScrobbles;
	char* TotalScrobbles;
	char* Genre;
	char* UserName;
	char* UserAvatar;
	char* UserArtistCount;
	char* UserScrobbles;
	char* UserFavourites;

	PreviousTrack PrevTracks[6];
};

LastfmScrobbleEntry entry;

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


//void Update(long timePassed)
//{
//	// Indicate that we want to exit if we reach the max frame count
//	if (RenderedFrameCount >= MaxFrameCountBeforeExit)
//	{
//		std::cout << "Exiting because max frame count has been reached." << std::endl;
//		GIsExiting = true;
//		return;
//	}
//
//	// Indicate that we want to exit if we didnt reach the max frame count but we raeched max seconds
//	if (RenderedFrameCount < MaxFrameCountBeforeExit && timePassed >= MaxSecondsBeforeExit)
//	{
//		std::cout << "Exiting because timeout. FrameCount:" << RenderedFrameCount << std::endl;
//		GIsExiting = true;
//	}
//}

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

		if (first_frame_rendered)
			GIsExiting = true;
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

private:
	bool first_frame_rendered;
	bool exitReceived;
};




class AppDelegate : public Cef3D::Cef3DAppDelegate
{
public:
	virtual void OnBeforeChildProcessLaunch(Cef3D::Cef3DCommandLine& CmdLine) override
	{
		CmdLine.AppendSwitchWithValue("song_name", entry.SongName);
		CmdLine.AppendSwitchWithValue("artist_name", entry.ArtistName);
		CmdLine.AppendSwitchWithValue("artist_scrobbles", entry.ArtistScrobbles);
		CmdLine.AppendSwitchWithValue("album_scrobbles", entry.AlbumScrobbles);
		CmdLine.AppendSwitchWithValue("total_scrobbles", entry.TotalScrobbles);
		CmdLine.AppendSwitchWithValue("album_cover", entry.AlbumCover);
		CmdLine.AppendSwitchWithValue("artist_cover", entry.ArtistCover);
		CmdLine.AppendSwitchWithValue("genre", entry.Genre);
		CmdLine.AppendSwitchWithValue("user_name", entry.UserName);
		CmdLine.AppendSwitchWithValue("user_avatar", entry.UserAvatar);
		CmdLine.AppendSwitchWithValue("user_artist_count", entry.UserArtistCount);
		CmdLine.AppendSwitchWithValue("user_scrobbles", entry.UserScrobbles);
		CmdLine.AppendSwitchWithValue("user_favourites", entry.UserFavourites);

		// 6 prev tracks
		for (int i = 0; i < 6; i++)
		{
			std::string cover = "prev_track_";
			cover.append(std::to_string(i));
			cover.append("_cover");

			std::string text = "prev_track_";
			text.append(std::to_string(i)); 
			text.append("_text");

			CmdLine.AppendSwitchWithValue(cover, entry.PrevTracks[i].Cover);
			CmdLine.AppendSwitchWithValue(text, entry.PrevTracks[i].Text);
		}
	}
};

int main(int argc, char** argv)
{
	auto start = std::chrono::steady_clock::now();

	if (argc != 3 + 25)
	{
		std::cout << "Need 28 parameters. Got: " << argc << " 1: Target HTML, 2: Output PNG path" << std::endl;
		return 0;
	}

	// Read arguments from command line
	SourceHTMLTargetPath = argv[1];
	TargetPngPath = argv[2];

	entry.SongName = argv[3];
	entry.ArtistName = argv[4];
	entry.AlbumCover = argv[5];
	entry.ArtistScrobbles = argv[6];
	entry.AlbumScrobbles = argv[7];
	entry.TotalScrobbles = argv[8];
	entry.Genre = argv[9];
	entry.UserName = argv[10];
	entry.UserAvatar = argv[11];
	entry.UserArtistCount = argv[12];
	entry.UserScrobbles = argv[13];
	entry.UserFavourites = argv[14];
	entry.ArtistCover = argv[15];

	int j = 0;
	for (int i = 0;  i < 12; i += 2)
	{
		entry.PrevTracks[j].Cover = argv[16 + i];
		entry.PrevTracks[j].Text = argv[16 + i + 1];
		j++;
	}
	
	// Output the arguments
	std::cout << "---------------Cef3D-------------" << std::endl;
	std::cout << "Target URL:" << SourceHTMLTargetPath << std::endl;
	std::cout << "Target PNG:" << TargetPngPath << std::endl;
	std::cout << "SongName:" << entry.SongName << std::endl;
	std::cout << "ArtistName:" << entry.ArtistName << std::endl;
	std::cout << "ArtistCover:" << entry.ArtistCover<< std::endl;
	std::cout << "AlbumCover:" << entry.AlbumCover << std::endl;
	std::cout << "ArtistScrobbles:" << entry.ArtistScrobbles << std::endl;
	std::cout << "AlbumScrobbles:" << entry.AlbumScrobbles << std::endl;
	std::cout << "TotalScrobbles:" << entry.TotalScrobbles << std::endl;
	std::cout << "Genre:" << entry.Genre << std::endl;
	std::cout << "UserName:" << entry.UserName << std::endl;
	std::cout << "UserAvatar:" << entry.UserAvatar << std::endl;
	std::cout << "UserArtistCount:" << entry.UserArtistCount << std::endl;
	std::cout << "UserScrobbles:" << entry.UserScrobbles << std::endl;
	std::cout << "UserFavourites:" << entry.UserFavourites << std::endl;
	for (int i = 0; i < 6; i++)
	{
		std::cout << "PrevTrack" << i << "_Cover:" << entry.PrevTracks[i].Cover << std::endl;
		std::cout << "PrevTrack" << i << "_Text:" << entry.PrevTracks[i].Text << std::endl;
	}


	/* Initialize Cef3D */
	bool isSubProcessed = true;

	Cef3D::Cef3DDefinition definition;
	definition.UseChildProcess = isSubProcessed;
	definition.OffscreenRendering = true;
	definition.UseCefLoop = false;

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