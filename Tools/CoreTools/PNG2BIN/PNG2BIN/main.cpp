//
//  main.c
//  PNGDotExtractor
//
//  Created by JodyMcAdams on 11/10/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#define PNG_DEBUG
#include "Engine/libpng/png.h"
#include "Engine/MathTypes.h"
#include "Engine/MathUtil.h"
#include "Engine/GraphicsTypes.h"

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <fstream>

typedef struct
{
	int width;
	int height;
	png_structp pPng;
	png_infop pInfo;
	png_bytep* pRows;
} PNGData;

typedef enum
{
	PNGDotExtractorArg_InputFileMask = 1,
	PNGDotExtractorArg_OutputFile,
}PNGDotExtractorArg;

void abort_(const char * s, ...);
bool read_png_file(const char* file_name, PNGData* pOut_Data);
bool process_file(const PNGData* pData, const char* outputFileName);

int main (int argc, const char * argv[])
{
	if(argc != 3)
	{
		std::cout << "Usage: PNG2BIN inputFileMask outputFile\n";
		std::cout << "Expected 2 arguments, " << argc-1 <<  " provided.\n";
		return -1;
	}
	
	//READING IN PNG FILES
	std::vector<std::string> fileListing;
	
	std::string fileMask;
	
	int lastSlashLoc = 0;
	
	const char* fileMaskChar = argv[PNGDotExtractorArg_InputFileMask];
	const size_t lengthOfFileMaskString = strlen(fileMaskChar);
	
	for(int strIDX=0; strIDX<lengthOfFileMaskString; ++strIDX)
	{
		const char currChar = fileMaskChar[strIDX];
		
		if(currChar == '/')
		{
			fileMask.clear();
			lastSlashLoc = strIDX;
		}
		else
		{
			fileMask += currChar;
		}
	}
	
	std::string inputFileMask(argv[PNGDotExtractorArg_InputFileMask]);
	std::string fileDir(inputFileMask.substr(0,lastSlashLoc));
	
	if(fileDir.size() == 0)
	{
		fileDir = "./";
	}
	
	//std::cout << fileDir << '\n';
	
	//Get list of files in mask directory
	DIR* pDir = opendir(fileDir.c_str());
    
    if(pDir == NULL)
	{
		std::cout << "Error: could not open directory specified in file mask: (" << fileDir << ")\n";
		return -1;
    }
	
	dirent* pDirent;
    while ((pDirent = readdir(pDir)) != NULL)
	{
		bool passedTest = true;
		const int length = std::min((int)fileMask.size(), (int)strlen(pDirent->d_name));
		for(int i=0; i<length; ++i)
		{
			if(fileMask[i] != '*' && pDirent->d_name[i] != fileMask[i])
			{
				passedTest = false;
				break;
			}
		}
		
		if(passedTest)
		{
			fileListing.push_back(std::string(pDirent->d_name));
		}
    }
    closedir(pDir);
	
	//std::cout << "\nFiles:\n";
	
	PNGData pngStruct;
	
	//Open up each PNG file found
	for(int i=0; i<fileListing.size(); ++i)
	{
		std::string fileName = fileDir + fileListing[i];
		
		if(read_png_file(fileName.c_str(), &pngStruct))
		{
			if(!process_file(&pngStruct,argv[PNGDotExtractorArg_OutputFile]))
			{
				std::cout << "Error: could not open output file for writing: (" << argv[PNGDotExtractorArg_OutputFile] << ")\n";
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	
	std::cout << "PNG2BIN: Conversion completed successfully!\n";
	
    return 0;
}



void abort_(const char * s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	//abort();
}


//png_sig_cmp(png_const_bytep sig, png_size_t start, png_size_t num_to_check)

bool read_png_file(const char* file_name, PNGData* pOut_Data)
{
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep* row_pointers;
	
	int height = 0;
	int width = 0;
	
	char header[8];    // 8 is the maximum size that can be checked
	
	/* open file and test for it being a png */
	FILE *fp = fopen(file_name, "rb");
	if (!fp)
	{
		abort_("[read_png_file] File %s could not be opened for reading", file_name);
		return false;
	}
	
	fread(header, 1, 8, fp);
	
	if (png_sig_cmp((png_const_bytep)header, (png_size_t)0, (png_size_t)8))
	{
		abort_("[read_png_file] File %s is not recognized as a PNG file", file_name);
		return false;
	}
	
	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png_ptr)
	{
		abort_("[read_png_file] png_create_read_struct failed");
		return false;
	}
	
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		abort_("[read_png_file] png_create_info_struct failed");
		return false;
	}
	
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		abort_("[read_png_file] Error during init_io");
		return false;
	}
	
	
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);
	
	png_read_info(png_ptr, info_ptr);
	
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	
	//number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);
	
	
	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		abort_("[read_png_file] Error during read_image");
		return false;
	}
	
	
	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (int y=0; y<height; y++)
		row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
	
	png_read_image(png_ptr, row_pointers);
	
	fclose(fp);
	
	pOut_Data->pInfo = info_ptr;
	pOut_Data->pRows = row_pointers;
	pOut_Data->pPng = png_ptr;
	pOut_Data->height = height;
	pOut_Data->width = width;
	
	return true;
}


bool process_file(const PNGData* pData, const char* outputFileName)
{
	std::ofstream myFile (outputFileName,std::fstream::trunc|std::fstream::binary);
	
	if(!myFile.is_open())
	{
		return false;
	}
	
	CoreImageHeader imageHeader;
	
	png_structp png_ptr = pData->pPng;
	png_infop info_ptr = pData->pInfo;
	png_bytep* row_pointers = pData->pRows;
	
	png_byte colorType = png_get_color_type(png_ptr, info_ptr);
	
	if (colorType != PNG_COLOR_TYPE_RGBA
		&& colorType != PNG_COLOR_TYPE_RGB)
		abort_("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGB or PNG_COLOR_TYPE_RGBA(%d) (is %d)",
			   PNG_COLOR_TYPE_RGBA, colorType);
	
	int numBytesPerPixel = 0;
	
	switch(colorType)
	{
		case PNG_COLOR_TYPE_RGB:
		{
			numBytesPerPixel = 3;
			break;
		}
		case PNG_COLOR_TYPE_RGBA:
		{
			numBytesPerPixel = 4;
			break;
		}
		default:
		{
			break;
		}
	}
	
	imageHeader.width = pData->width;
	imageHeader.height = pData->height;
	imageHeader.flags = colorType == PNG_COLOR_TYPE_RGBA ? 1 : 0;
	imageHeader.dataSize = pData->width*pData->height*2;
	
	u16* pOutputData = new u16[imageHeader.width*imageHeader.height];
	
	myFile.write((const char*)&imageHeader, sizeof(imageHeader));
	
	
	/*const int height = pData->height;
	const int width = pData->width;
	
	for (int y=0; y<height; y++)
	{
		png_byte* row = row_pointers[y];
		
		for (int x=0; x<width; x++)
		{
			png_byte* ptr = &(row[x*numBytesPerPixel]);
			
			const unsigned char R = ptr[0];
			const unsigned char G = ptr[1];
			const unsigned char B = ptr[2];
			
			for(int i=0; i<g_PixelPosData.size(); ++i)
			{
				PixelPosData* pCurrData = &g_PixelPosData[i];
				if(pCurrData->processedAlready)
				{
					continue;
				}
				
				if(pCurrData->R == R
				   && pCurrData->G == G
				   && pCurrData->B == B)
				{
					const f32 xT = ((float)x/(float)(width-1))*2.0f-1.0f;
					const f32 yT = ((float)y/(float)(height-1))*2.0f-1.0f;
					
					const vec2 outVec = {xT,yT};
					pCurrData->pixelPositions.push_back(outVec);
					
					pCurrData->processedAlready = true;
					
					//printf("Found a magic pixel <%d,%d,%d> at (%f, %f)!\n",R,G,B,xT,yT);
					
					break;
				}
			}
		}
	}*/
	
	myFile.write((const char*)pOutputData, imageHeader.dataSize);
	
	myFile.close();
	
	return true;
}

