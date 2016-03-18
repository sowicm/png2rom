
#include <stdio.h>
#include <png.h>
#include <pnglibconf.h>
#include <pngconf.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

void output(char* data);


void putchar_test(unsigned char c)
{
	printf("%d\t",  c);
}

/*
#undef putchar
#define putchar(x) putchar_test(x)
*/

void test();
void encode();
void preview();

int main()
{
	encode();
	//preview();
}

struct _8bits
{
	unsigned char data;
	/*
	union {
		unsigned char data;
		struct __8bits {
		unsigned int b0 : 1;
		unsigned int b1 : 1;
		unsigned int b2 : 1;
		unsigned int b3 : 1;
		unsigned int b4 : 1;
		unsigned int b5 : 1;
		unsigned int b6 : 1;
		unsigned int b7 : 1;
		};
	};
*/
	void set(int i, int v)
	{
		if (v == 1)
		{
			data |= 1 << i;
		}
		else if (v == 0)
		{
			data &= ~(1 << i);
		}
	}
};

void encode()
{
	png_structp png_ptr;
	png_infop    info_ptr;
	unsigned int sig_read = 0;
	FILE *fp;
	FILE *fw = fopen("rom.dat", "wb");
	char idata[256];
	_8bits odata[32];

	assert(sizeof(_8bits) == sizeof(char));
	assert(sizeof(odata) == 32);
	
	char path[256];
	for (int iA = 1; iA <= 16; ++iA)
	{
		for (int iF = 1; iF <= 16; ++iF)
		{
			sprintf(path, "/home/sowicm/s/[home]/documents/数电/anim/auto/%d/%d_ab.png", iA, iF);
			if (!(fp = fopen(path, "rb")))
			{
				printf("read %s failed\n", path);
				continue;
			}
		    png_ptr = png_create_read_struct(
	                                 PNG_LIBPNG_VER_STRING,
									 0,
	                                 0, 
	                                 0);

			if (png_ptr == NULL)
			{
				fclose(fp);
				printf("%s: create read struct failed!", path);
				continue;
			}

			info_ptr = png_create_info_struct(png_ptr);
			if (info_ptr == NULL)
			{
				fclose(fp);
				png_destroy_read_struct(
		                        &png_ptr, 
		                        0,
		                        0);
				printf("%s: create info struct failed", path);
				continue;
			}

			if (setjmp(png_jmpbuf(png_ptr)))
			{
				png_destroy_read_struct(
		                        &png_ptr,
		                        &info_ptr,
		                        0);
				printf("%s: setjmp failed!", path);
				continue;
			}
			png_init_io(png_ptr, fp);
			png_set_sig_bytes(png_ptr, sig_read);
			png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

			png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);

			char *p = idata;
			for (int i = 0; i < 16; ++i)
			{
				for (int j = 0; j < 32; j += 2)
				{
					*p++ = row_pointers[i][j];
				}
			}

			p = idata;
			for (int j = 0; j < 8; ++j)
			{
				for (int i = 0; i < 8; ++i)
	    			odata[i * 4].set(j, *p++ == 0);
				for (int i = 0; i < 8; ++i)
					odata[i * 4 + 1].set(j, *p++ == 0);
			}	
			for (int j = 0; j < 8; ++j)
			{
				for (int i = 0; i < 8; ++i)
					odata[i * 4 + 2].set(j, *p++ == 0);
				for (int i = 0; i < 8; ++i)
					odata[i * 4 + 3].set(j, *p++ == 0);
			}
			fwrite(odata, sizeof(odata), 1, fw);
	
			png_destroy_read_struct(
	                        &png_ptr,
	                        &info_ptr,
	                        NULL);

			fclose(fp);
		}
	}
}

void preview()
{
	FILE *fp = fopen("rom.dat", "rb");
	int c;
	unsigned char odata[256 * 32];
	fread(odata, sizeof(odata), 1, fp);
	fclose(fp);

	for (int iF = 0; iF < 256; ++iF)
	{
		for (int j = 0; j < 8; ++j)
		{
			for (int i = 0; i < 8; ++i)
				putchar( (odata[iF * 32 + i * 4] & (unsigned char)(1 << j)) ? '+' : ' ' );
			for (int i = 0; i < 8; ++i)
				putchar( (odata[iF * 32 + i * 4 + 1] & (unsigned char)(1 << j)) ? '+' : ' ');
			puts("");
		}
		for (int j = 0; j < 8; ++j)
		{
			for (int i = 0; i < 8; ++i)
				putchar( (odata[iF * 32 + i * 4 + 2] & (unsigned char)(1 << j)) ? '+' : ' ');
			for (int i = 0; i < 8; ++i)
				putchar( (odata[iF * 32 + i * 4 + 3] & (unsigned char)(1 << j)) ? '+' : ' ');
			puts("");
		}
		usleep(200000);
		system("clear");
	}
}

void test()
{
	
	char idata[256];
	char odata[256];
	for (int i = 0; i < 256; ++i)
		idata[i] = i;


	char *p = idata;
	for (int j = 0; j < 8; ++j)
	{
		for (int i = 0; i < 8; ++i)
	        odata[i * 32 + j] = *p++;
		for (int i = 0; i < 8; ++i)
			odata[i * 32 + j + 8] = *p++;
	}	
	for (int j = 0; j < 8; ++j)
	{
		for (int i = 0; i < 8; ++i)
			odata[i * 32 + j + 16] = *p++;
		for (int i = 0; i < 8; ++i)
			odata[i * 32 + j + 24] = *p++;
	}
	
	
	output(odata);
	getchar();
}

void output(char* data)
{
	for (int j = 0; j < 8; ++j)
	{
		for (int i = 0; i < 8; ++i)
			putchar(data[i * 32 + j]);
		for (int i = 0; i < 8; ++i)
			putchar(data[i * 32 + j + 8]);
		puts("");
	}	
	for (int j = 0; j < 8; ++j)
	{
		for (int i = 0; i < 8; ++i)
			putchar(data[i * 32 + j + 16]);
		for (int i = 0; i < 8; ++i)
			putchar(data[i * 32 + j + 24]);
		puts("");
	}
}
