// FontToTexture.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>

#include "Engine/MathTypes.h"
#include "Engine/MathUtil.h"
#include "Engine/GRPNG/writepng.h"

#include <iostream>
#include <fstream>

void PauseBeforeExit()
{
	#ifdef _DEBUG
		printf("Press any key...\n");
		getch();
	#endif
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 4)
	{
		printf("Useage:  FontToTexture filename.ttf fontsize(pixels) texturesize(pixels) padding(pixels) outputfilename.png outputfilename.bin\nExiting...\n");
		PauseBeforeExit();
		return -1;
	}	

	FT_Library  library;

	{
		FT_Error error = FT_Init_FreeType( &library );
		if ( error )
		{
			printf("ERROR: Failed to initialize FreeType Library! Exiting...\n");
			PauseBeforeExit();
			return -1;
		}
		else
		{
			printf("FreeType Library Initialized!\n");
		}
	}

	FT_Face face;

	{
		FT_Error error = FT_New_Face( library,
                       argv[1],
                       0,
                       &face );

		if ( error == FT_Err_Unknown_File_Format )
		{
			printf("ERROR: Unsupported font format! Exiting...\n");
			PauseBeforeExit();
			return -1;
		}
		else if ( error )
		{
			printf("ERROR: error reading font file! Exiting...\n");
			PauseBeforeExit();
		}
		else
		{
			printf("Loaded Font: %s\n",argv[1]);
		}
	}

	printf("Outputting %s...\n",argv[5]);

	const s32 inputTextureSize = atoi(argv[3]);
	const s32 textureSize = PowerOf2(inputTextureSize);

	if(textureSize != inputTextureSize)
	{
		printf("Warning: Texture size %d is not a power of 2.  Setting texture size to %d...\n",inputTextureSize,textureSize);
	}

	const u32 maxTextureIndex = textureSize-1;

	const s32 padding = atoi(argv[4]);

	//Multiply by 64 to counter freetype's 1/64th pixel scale
	const s32 fontSize = atoi(argv[2])*64;

	const u32 fontBufferSize = textureSize*textureSize;
	u8* pFontBuffer = new u8[fontBufferSize];
	memset(pFontBuffer,0,fontBufferSize);
	u32 currX = padding;
	u32 currY = padding;

	//Apparently a normal DPI size for a screen is 96
	const u32 deviceDPI = 96;
	FT_Set_Char_Size( face, fontSize, fontSize, deviceDPI, deviceDPI);

	u32 maxHeightForRow = 0;

	//Open binary file for writing out offsets
	std::ofstream binaryFile;
	binaryFile.open (argv[6], std::ios::out | std::ios::trunc | std::ios::binary);
	if(!binaryFile.is_open())
	{
		printf("ERROR: failed to open file '%s' for writing! Exiting...\n",argv[6]);
		PauseBeforeExit();
		return -1;
	}

	for(u32 i=32; i<127; ++i)
	{
		// Load The Glyph For Our Character.
		if(FT_Load_Glyph( face, FT_Get_Char_Index( face, i ), FT_LOAD_DEFAULT ))
		{
			printf("Warning: Failed to load glyph for character %c, skipping...\n",i);
			continue;
		}

		// Move The Face's Glyph Into A Glyph Object.
		FT_Glyph glyph;
		if(FT_Get_Glyph( face->glyph, &glyph ))
		{
			printf("Warning: Failed to move glyph into glyph object for character %c, skipping...\n",i);
			continue;
		}

		// Convert The Glyph To A Bitmap.
		FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
 
		// This Reference Will Make Accessing The Bitmap Easier.
		FT_Bitmap& bitmap=bitmap_glyph->bitmap;

		const s32 width = bitmap.width;
		const s32 height = bitmap.rows;

		const u32 maxX = currX+width+padding;
		
		if(maxX >= textureSize)
		{
			currX = padding;
			currY += maxHeightForRow+padding;
			maxHeightForRow = 0;

			if(currY >= textureSize)
			{
				binaryFile.close();
				printf("ERROR: all the characters cannot fit onto the texture! Exiting...\n",argv[5]);
				PauseBeforeExit();

				return -1;
			}
		}

		if(height > maxHeightForRow)
		{
			maxHeightForRow = bitmap.rows;
		}

		//write out current offsets to binary file
		binaryFile << (f32)(currX+padding)/(f32)maxTextureIndex;
		binaryFile << (f32)(currY+padding)/(f32)maxTextureIndex;
		binaryFile << (f32)(currX+width-padding)/(f32)maxTextureIndex;
		binaryFile << (f32)(currY+height-padding)/(f32)maxTextureIndex;

		//TODO: might need some kind of height adjustment output I'm not sure yet
 
		// Here We Fill In The Data For The Expanded Bitmap.
		// Notice That We Are Using A Two Channel Bitmap (One For
		// Channel Luminosity And One For Alpha), But We Assign
		// Both Luminosity And Alpha To The Value That We
		// Find In The FreeType Bitmap.
		// We Use The ?: Operator To Say That Value Which We Use
		// Will Be 0 If We Are In The Padding Zone, And Whatever
		// Is The FreeType Bitmap Otherwise.
		for(int y=0; y <height;y++)
		{
			for(int x=0; x < width; x++)
			{
				const u8 pixelValue = bitmap.buffer[x + bitmap.width*y];
				pFontBuffer[currX+x + textureSize*(y+currY)] = pixelValue;
				//printf("pixelvalue: %d\n",pixelValue);
			}
		}

		currX += width+padding;
	}

	//We're done with this file
	binaryFile.close();

	FILE* pOutputFile = fopen(argv[5], "wb");
	if(pOutputFile == NULL)
	{
		printf("ERROR: failed to open file '%s' for writing! Exiting...\n",argv[5]);
		PauseBeforeExit();

		return -1;
	}

	mainprog_info pngInfo;
	pngInfo.pnmtype = 5;
	pngInfo.gamma = 2.22;
	pngInfo.height = textureSize;
	pngInfo.width = textureSize;
	pngInfo.sample_depth = 8;
	pngInfo.have_time = 0;
	pngInfo.have_text = 0;
	pngInfo.have_bg = 0;
	pngInfo.image_data = NULL;
	pngInfo.filter = FALSE;
	pngInfo.infile = NULL;
	pngInfo.outfile = pOutputFile;
	pngInfo.interlaced = 0;

	if(writepng_init(&pngInfo) != 0)
	{
		fclose(pOutputFile);
		printf("ERROR: initializing PNG struct! Exiting...\n");
		PauseBeforeExit();
	}

	const u32 rowBytes = textureSize;
	
	for(u32 y=0; y<textureSize; ++y)
	{
		pngInfo.image_data = &pFontBuffer[textureSize*y];

		if(writepng_encode_row(&pngInfo) != 0)
		{
			fclose(pOutputFile);
			printf("ERROR: libpng error encoding PNG! Exiting...\n");
			writepng_cleanup(&pngInfo);
			PauseBeforeExit();
		}
	}


	if(writepng_encode_finish(&pngInfo) != 0)
	{
		fclose(pOutputFile);
		printf("ERROR: error finalizing the PNG! Exiting...\n");
		writepng_cleanup(&pngInfo);
		PauseBeforeExit();
	}
	
	//delete[] rowBuffer;

	writepng_cleanup(&pngInfo);

	fclose(pOutputFile);

	printf("Finished!\n");
	PauseBeforeExit();

	return 0;
}

