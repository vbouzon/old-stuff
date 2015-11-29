//
//	tga.cpp - source file / freeware
//
//	David Henry - tfc_duke@club-internet.fr
//


#include	<fstream>
#include	"tga.h"



// --------------------------------------------------
// LoadFileTGA() - charge une image TrueVision
//				   TARGA [.tga].
//
// paramètres :
//    - filename [in]  : nom de fichier
//    - pixels	 [out] : données image rgba 32 bits
//    - width    [out] : largeur en pixels
//    - height   [out] : hauteur en pixels
//    - flipvert [in]  : flag retournement vertical
//
// valeur de retour :
//    - -1 : pas de données pixels
//    -  0 : échec
//    -  1 : succès
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// formats acceptés:
//     # RGB 8-16-24-32 bits
//     # RLE 8-16-24-32 bits
// --------------------------------------------------

int LoadFileTGA( const char *filename, unsigned char **pixels, int *width, int *height, bool flipvert )
{
	std::ifstream	file;			// fichier
	TGAHEADER		*tgah;			// header targa
	RGBTRIPLE		*palette;		// pointeur palette couleur
	char			*buffer;		// buffer stockant l'intégralité du fichier
	unsigned char	*ptr;			// pointeur données pixels
	int				row, col, i;	// variables de contrôle



	/////////////////////////////////////////////////////
	// lit l'intégralité du fichier dans le buffer

	file.open( filename, std::ios::in | std::ios::binary );

	if( file.fail() )
		return 0;

	file.seekg( 0, std::ios::end );
	long flen = file.tellg();
	file.seekg( 0, std::ios::beg );

	buffer = new char[ flen + 1 ];
	file.read( buffer, flen );
	char *pBuff = buffer;

	file.close();

	/////////////////////////////////////////////////////


	// on lit le header
	tgah = (TGAHEADER *)pBuff;
	pBuff += sizeof( TGAHEADER );


	if( width )
		*width = tgah->is_width;

	if( height )
		*height = tgah->is_height;

	if( !pixels )
	{
		delete [] buffer;
		return (-1);
	}


	// allocation de mémoire pour stocker les données pixels
	*pixels	= new unsigned char[ tgah->is_width * tgah->is_height * 4 ];
	ptr		= &(*pixels)[0];


	// on déplace le pointeur fichier au début des données pixels
	if( tgah->id_lenght )
		pBuff = buffer + (tgah->id_lenght * sizeof( unsigned char ));


	/////////////////////////////////////////////////////
	// lecture de la palette

	if( tgah->color_map_type )
	{
		// les images 24 et 32 bits ne possèdent pas de palettes.

		// on ajuste le pointeur de la palette sur la zone mémoire
		// correspondante dans buffer
		palette = (RGBTRIPLE *)pBuff;

		pBuff += tgah->cm_length * (tgah->cm_size >> 3) * sizeof( unsigned char );
	}
	else
	{
		palette = 0;
	}

	/////////////////////////////////////////////////////


	/////////////////////////////////////////////////////
	// lecture des données suivant le type de
	// compression et le nombre de bits par pixel
	/////////////////////////////////////////////////////


	switch( tgah->image_type )
	{
		case 0:
			// pas de données image
			break;

		case 1:
// COLOR-MAPPED BGR 8 BITS GREYSCALE

		case 3:
		{
// COLOR-MAPPED BGR 8 BITS
			for( row = tgah->is_height - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * tgah->is_width * 4 ];

				for( col = 0; col < tgah->is_width; col++, ptr += 4 )
				{
					// lecture du pixel en cours
					unsigned char color = *((unsigned char *)(pBuff++));

					// conversion pixel index couleur bgr en pixel rgba 32 bits
					ptr[0] = palette[ color ].rgbtRed;		// b->r
					ptr[1] = palette[ color ].rgbtGreen;	// g->g
					ptr[2] = palette[ color ].rgbtBlue;		// r->b
					ptr[3] = 255;							// alpha
				}
			}

			break;
		}

		case 2:
		{
			for( row = tgah->is_height - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * tgah->is_width * 4 ];

				for( col = 0; col < tgah->is_width; col++, ptr += 4 )
				{
					switch( tgah->is_pixel_depth )
					{
						case 16:
						{
// TRUE-COLOR BGR 16 BITS
							// lecture du pixel en cours
							unsigned short color = *((unsigned short *)pBuff);
							pBuff += sizeof( short );

							// convertion pixel rgb 16 bits en rgb 32 bits
							ptr[0] = ((color & 0x7C00) >> 10) << 3;	// b->r
							ptr[1] = ((color & 0x03E0) >>  5) << 3;	// g->g
							ptr[2] = ((color & 0x001F) >>  0) << 3;	// r->b
							ptr[3] = 255;							// alpha

							break;
						}

						case 24:
						{
// TRUE-COLOR BGR 24 BITS
							// conversion pixel bgr 24 bits en pixel rgba 32 bits
							RGBTRIPLE *pix = (RGBTRIPLE *)pBuff;
							pBuff += sizeof( RGBTRIPLE );

							ptr[0] = pix->rgbtRed;
							ptr[1] = pix->rgbtGreen;
							ptr[2] = pix->rgbtBlue;
							ptr[3] = 255;

							break;
						}

						case 32:
						{
// TRUE-COLOR BGR 32 BITS
							// conversion pixel bgr 32 bits en pixel rgba 32 bits
							BGRAQUAD *pix = (BGRAQUAD *)pBuff;
							pBuff += sizeof( BGRAQUAD );

							ptr[0] = pix->bgraRed;
							ptr[1] = pix->bgraGreen;
							ptr[2] = pix->bgraBlue;
							ptr[3] = pix->bgraAlpha;

							break;
						}
					}
				}
			}

			break;
		}

		case 9:
// RLE COLOR-MAPPED BGR 8 BITS

		case 11:
		{
// RLE COLOR-MAPPED BGR 8 BITS GREYSCALE

			unsigned char	packetHeader, packetSize, i;


			for( row = tgah->is_height - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * tgah->is_width * 4 ];

				for( col = 0; col < tgah->is_width; /* rien */ )
				{
					packetHeader	= *((unsigned char *)(pBuff++));
					packetSize		= 1 + (packetHeader & 0x7f);

					if( packetHeader & 0x80 )
					{
						// run-length packet

						// lecture du pixel en cours
						unsigned char color = *((unsigned char *)(pBuff++));

						// conversion pixel index couleur bgr en pixel rgba 32 bits
						for( i = 0; i < packetSize; i++, ptr += 4, col++ )
						{
							ptr[0] = palette[ color ].rgbtRed;		// b->r
							ptr[1] = palette[ color ].rgbtGreen;	// g->g
							ptr[2] = palette[ color ].rgbtBlue;		// r->b
							ptr[3] = 255;							// alpha
						}
					}
					else
					{
						// non run-length packet

						for( i = 0; i < packetSize; i++, ptr += 4, col++ )
						{
							// lecture du pixel en cours
							unsigned char color = *((unsigned char *)(pBuff++));

							// conversion pixel index couleur bgr en pixel rgba 32 bits
							ptr[0] = palette[ color ].rgbtRed;		// b->r
							ptr[1] = palette[ color ].rgbtGreen;	// g->g
							ptr[2] = palette[ color ].rgbtBlue;		// r->b
							ptr[3] = 255;							// alpha
						}
					}
				}
			}

			break;
		}

		case 10:
		{
			unsigned char	packetHeader, packetSize;


			for( row = tgah->is_height - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * tgah->is_width * 4 ];

				for( col = 0; col < tgah->is_width; /* rien */ )
				{
					packetHeader	= *((unsigned char *)(pBuff++));
					packetSize		= 1 + (packetHeader & 0x7f);

					if( packetHeader & 0x80 )
					{
						// run-length packet
						switch( tgah->is_pixel_depth )
						{
							case 16:
							{
// RLE TRUE-COLOR BGR 16 BITS
								// lecture du pixel en cours
								unsigned short color = *((unsigned short *)pBuff);
								pBuff += sizeof( short );

								// convertion pixel rgb 16 bits en rgb 32 bits
								for( i = 0; i < packetSize; i++, ptr += 4, col++ )
								{
									ptr[0] = ((color & 0x7C00) >> 10) << 3;	// b->r
									ptr[1] = ((color & 0x03E0) >>  5) << 3;	// g->g
									ptr[2] = ((color & 0x001F) >>  0) << 3;	// r->b
									ptr[3] = 255;
								}

								break;
							}

							case 24:
							{
// RLE TRUE-COLOR BGR 24 BITS
								// conversion pixel bgr 24 bits en pixel rgba 32 bits
								RGBTRIPLE *pix = (RGBTRIPLE *)pBuff;
								pBuff += sizeof( RGBTRIPLE );

								for( i = 0; i < packetSize; i++, ptr += 4, col++ )
								{
									ptr[0] = pix->rgbtRed;
									ptr[1] = pix->rgbtGreen;
									ptr[2] = pix->rgbtBlue;
									ptr[3] = 255;
								}

								break;
							}

							case 32:
							{
// RLE TRUE-COLOR BGR 32 BITS
								// conversion pixel bgr 32 bits en pixel rgba 32 bits
								BGRAQUAD *pix = (BGRAQUAD *)pBuff;
								pBuff += sizeof( BGRAQUAD );

								for( i = 0; i < packetSize; i++, ptr += 4, col++ )
								{
									ptr[0] = pix->bgraRed;
									ptr[1] = pix->bgraGreen;
									ptr[2] = pix->bgraBlue;
									ptr[3] = pix->bgraAlpha;
								}

								break;
							}
						}
					}
					else
					{
						// non run-length packet

						for( i = 0; i < packetSize; i++, ptr += 4, col++ )
						{
							switch( tgah->is_pixel_depth )
							{
								case 16:
								{
// RLE TRUE-COLOR BGR 16 BITS
									// lecture du pixel en cours
									unsigned short color = *((unsigned short *)pBuff);
									pBuff += sizeof( short );

									// convertion pixel rgb 16 bits en rgb 32 bits
									ptr[0] = ((color & 0x7C00) >> 10) << 3;	// b->r
									ptr[1] = ((color & 0x03E0) >>  5) << 3;	// g->g
									ptr[2] = ((color & 0x001F) >>  0) << 3;	// r->b
									ptr[3] = 255;							// alpha

									break;
								}

								case 24:
								{
// RLE TRUE-COLOR BGR 24 BITS
									// conversion pixel bgr 24 bits en pixel rgba 32 bits
									RGBTRIPLE *pix = (RGBTRIPLE *)pBuff;
									pBuff += sizeof( RGBTRIPLE );

									ptr[0] = pix->rgbtRed;
									ptr[1] = pix->rgbtGreen;
									ptr[2] = pix->rgbtBlue;
									ptr[3] = 255;

									break;
								}

								case 32:
								{
// RLE TRUE-COLOR BGR 32 BITS
									// conversion pixel bgr 32 bits en pixel rgba 32 bits
									BGRAQUAD *pix = (BGRAQUAD *)pBuff;
									pBuff += sizeof( BGRAQUAD );

									ptr[0] = pix->bgraRed;
									ptr[1] = pix->bgraGreen;
									ptr[2] = pix->bgraBlue;
									ptr[3] = pix->bgraAlpha;

									break;
								}
							}
						}
					}
				}
			}

			break;
		}

		default:
		{
			// format inconnu ou non traîté
			delete [] pixels;
			delete [] buffer;
			return 0;
		}
	}


	// désallocation mémoire tampon
	delete [] buffer;

	// succès
	return 1;
}
