/*** The programe reads out image data from image file and write into it ***/
// here I used gray scale image "lena.raw" as an example.
// it will read the image data to Imagedata array
// it also write data in Imagedata array to image file "lena1.raw" 
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <string.h>
#include <math.h>

// define image size 512*512
// you can change it to other size
#define Size 256 

	/******        	->		Notice : which one  is  Size,     ******/
	/******  					256, 							 		  	 ******/
	/****** 					   or 512, 							   	 ******/
	/****** 					   or others?  					   	 ******/

int get_hist( unsigned char imgArr[][ Size ], int histInitArr[ 256 ] ){

	int max = 0;
	int min = 0;
	int i = 0;
	int j = 0;
	printf( "\n\n\n\n\nmin is %d, max is %d.", min, max );
	/*						->	  pixel occur. for each graylevel init. to 0 */
	
	for( i = 0; i < 256; i++ ){
		histInitArr[ i ] = 0;	
	}


	//						->	  get the pixel-occurence for each gray level
	int temp_index[Size][Size];
	int temp = 0;
	for( i = 0; i < Size; i++ ){
		for( j = 0; j < Size; j++ ){
			temp_index[i][j] = (int)imgArr[i][j];
			temp = temp_index[i][j];
			histInitArr[ temp ]++; 
			/* for each graylevel = imgArr[i][j], count pixel occurence */
		} // End inner-for
	} // End outer-for
	
	for( i = 1; i < 4; i++){
		for( j = 1; j < 4; j++ ){
			printf( "temp_index is %d.", temp_index[i][j] );
			printf( "\nhistInitArr %d is %d.\n\n", i, histInitArr[ i ] );
		}
	}
	/****************************/
	for( i = 0; i <= 256; i++ ){
		if( histInitArr[ i ] > max ){
			max = histInitArr[ i ];	
		} // End if 
		if( histInitArr[ i ] < min ){
			min = histInitArr[ i ];
	
		} // End if 
	} // End for
	
	//						->		normalize histInitArr
	
	
	
	/***************************/	
	printf( "\n\n\n\n\n ******* ->   min is %d, max is %d.\n\n", min, max );	
	
   for(int x=100; x<104; x++){
		for(int y=100;y<103;y++){
			printf("\nimgArr[] : %d", imgArr[x][y] );
		}	
	}	
	for( int cc = 51; cc < 56; cc++ ){
			printf("\nin histInitArr[ %d ] : %d", cc, histInitArr[cc] );		
	}
} // End get_hist( )

/**********************************************************/


int equalize_hist( int histArr[ 256 ], unsigned char inputImg[][ Size ], unsigned char outputImg[][ Size ] )
{

	//						->		cdf = cummulative distribution function

	float cdfArr[ 256 ];
	float prob[ 256 ];
	int i = 0;
	
	
	//						->		get probability for each graylevel
	
	for( int kk = 0; kk < 12; kk++ ){
		printf( "\n\nhistArr[ %d ] : %d.\n", kk, histArr[ kk ] );
	}
	
	for( i = 0; i < 256; i++ ){
		prob[ i ] =  (float) histArr[ i ] / ( Size*Size ) ;
	} // End for 
	
	for( i=0;i<6;i++){
		printf( "prob[ %d ] : %f.\n", i, prob[i] );
	}
	
	float cdfMax = -1;
	float cdfMin = 257;
	
	//						->		because cdf is  "prevPrev + prev"
	//								cdfArr[ 0 ] must have a value
	
	cdfArr[ 0 ] = prob[ 0 ];
	printf( "\n\n	Now cdfArr[0] is %f.\n\n", cdfArr[0] );
	//						->		get cumulative distribution
	
	for( i = 1; i < 256; i++ ){
		cdfArr[ i ] = prob[ i ] + cdfArr[ i - 1 ];	
		
	} // End for 
	
	for( int vv = 0; vv < 20; vv++ ){
		printf( "cdfArr[ %d ]: %f.  \n\n", vv, cdfArr[ vv ] );
	}
	printf( "\n\ncdfArr[ 255 ] : %f.\n\n\n", cdfArr[ 255 ] );
	
	
	for( i = 0; i < 256; i++ ){
		if( cdfArr[ i ] > cdfMax ){
			cdfMax = cdfArr[ i ];	
		} // End if 
		
		if( cdfArr[ i ] < cdfMin ){
			cdfMin = cdfArr[ i ];
		} // End if 
	} // End for 
	
	//						->		equalize the histogram	
	float temp;
	for( i = 0; i < Size; i++ ){
		for( int j = 0; j < Size; j++ ){
			temp = cdfArr[(int)inputImg[i][j] ];
			//temp = ceil(temp);
			outputImg[i][j] = (unsigned char)( 255 * temp );
			
		} // End outer-for
	} // End inner-for
		for( i = 0 ; i < 5; i++ ){
		for( int j = 0; j < 5; j++ ){
			printf("outputImg[%d][%d]  :  %d.\n", i, j, outputImg[i][j] );
		}
	}
	
	
} // End equalize_hist( )







int main(void)
{

	//						->	  file pointer

	FILE *file;

	// 					->   image data array

	unsigned char Imagedata[Size][Size];
	unsigned char img_D[ Size ][ Size ];	
	unsigned char img_H[ Size ][ Size ];
	unsigned char img_L[ Size ][ Size ];
	int hist_H[ 256 ];
	int i = 0;
	int j = 0;	
	
	/*******************************************************/
	
	//						>>   read image "lena.raw" into image data matrix

	if (!(file=fopen("sample1.raw","rb")))
	{
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size, file);
	fclose(file);
   for(int x=0; x<Size; x++){
		for(int y=0;y<Size;y++){
 			//printf("\n%d", Imagedata[x][y] );
		}	
	}



	/*******************************************************/
	
	// 				    >>	do some image processing task...	
	
	memcpy( img_D, Imagedata, sizeof(char)*Size*Size );
	for( i = 0; i < Size; i++ ){
		for( j = 0; j < Size; j++ ){
			img_D[ i ][ j ] = ( img_D[ i ][ j ] / 2 );	
		} // End for
	} // End for

	printf("\n\n\nimg_D wow ! \n\n");
   for(int x=0; x<10; x++){
		for(int y=0;y<2;y++){
			printf("\n%d", img_D[x][y] );
		}	
	}

	get_hist( img_D, hist_H );

	equalize_hist( hist_H, img_D, img_H );

	for( i = 0 ; i < 3; i++ ){
		for( j = 0; j < 2; j++ ){
			printf("img_H %d.\n", img_H[i][j] );
		}
	}	

	/*******************************************************/
	
	//						 >>   write image data to "D.raw"	
	
	if (!(file=fopen("D.raw","wb")))
	{ 	
		cout<<"Cannot open file!"<<endl;
			system("PAUSE");
		exit(1);
	}	
	//fwrite(Imagedata, sizeof(unsigned char), Size*Size, file);		
	for( i = 0; i < Size; i++ ){
		fwrite( img_D[ i ],  sizeof( unsigned char )  , Size, file );
	}
	
	/*******************************************************/
	//						 >>   write image data to "H.raw"	
	/***********************************/
	if (!(file=fopen("H.raw","wb")))
	{
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	for( i = 0; i < Size; i++ ){
		fwrite( img_H[ i ],  sizeof( unsigned char )  , Size, file );
	}
	fclose(file);
	exit(0);
	printf("\n\n-----------------------------\n\n\n");
	/******************************************************/
	/*******************************************************/
	
	//						 >>   write image data to "L.raw"	
	/**************************************************
	if (!(file=fopen("L.raw","wb")))
	{
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	for( i = 0; i < Size; i++ ){
		fwrite( img_L[ i ],  sizeof( unsigned char )  , Size, file );
	}
	fclose(file);
	exit(0);	
	*******************************************************/
	
	system("PAUSE");
	return 0;
} // End main ( ) 




	
