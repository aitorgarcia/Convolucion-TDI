#include <stdio.h>
#include <C_Image.hpp>
#include <C_Matrix.hpp>
#include <C_General.hpp>

C_Image imagenEntrada;
C_Image imagenSalida;
C_Matrix matrizConvolucion;
char nombreFiltro[80];


int CalcularValorPixel(int x, int y, int i, int j, C_Matrix matrizConvolucion);
void ReajusteValores();

void matrizFiltroLaplaciano5x5();
void matrizFiltroPasoAlto3x3();
void matrizFiltroPasoBajo3x3();
void matrizFiltroSobelHorizontal3x3();
void matrizFiltroSobelVertical3x3();
void matrizFiltroMedia3x3();
void matrizFiltroPasoAlto5x5();
void matrizFiltroGaussiano3x3();
void matrizFiltroGaussiano5x5();
void matrizFiltroLaplaciano3x3();
void matrizFiltroPrewittVertical3x3();
void matrizFiltroPrewittHorizontal3x3();

int main(int argc, char **argv)
{
	printf("################################################################\n");
	printf("#                                                              #\n");
	printf("#   APLICACION DE FILTROS MEDIANTE LA TECNICA DE CONVOLUCION   #\n");
	printf("#                                                              #\n");
	printf("#                                          Aitor Garcia Luiz   #\n");
	printf("################################################################\n");

	//Lectura de la imagen de entrada (si el archivo no existe, vuelve a pedirtelo):
	char nombreSinFormato[80] = "";

	do {
		char nombreImagen[80] = "";

		//Primero limpia la variable imagenEntrada, por si hubiera más de un intento no interfiera el intento fallido anterior
		imagenEntrada.Free();
		printf("\n\nIntroduce el nombre de la imagen (sin incluir el formato '.bmp'):\n");
		scanf("%s", nombreSinFormato);

		strcat(nombreImagen, nombreSinFormato);
		strcat(nombreImagen, ".bmp");

		imagenEntrada.ReadBMP(nombreImagen);
	} while (imagenEntrada.Empty());


	int opcion = 0;
	do {
		printf("\n\n");
		printf("########################################\n");
		printf("#          FILTROS APLICABLES          #\n");
		printf("########################################\n");
		printf("#  1. Filtro Media                     #\n");
		printf("#  2. Filtro Paso Bajo                 #\n");
		printf("#  3. Filtro Paso Alto                 #\n");
		printf("#  4. Filtro Paso Alto (5x5)           #\n");
		printf("#  5. Filtro Gaussiano                 #\n");
		printf("#  6. Filtro Gaussiano (5x5)           #\n");
		printf("#  7. Filtro Laplaciano                #\n");
		printf("#  8. Filtro Laplaciano (5x5)          #\n");
		printf("#  9. Filtro Sobel Horizontal          #\n");
		printf("# 10. Filtro Sobel Vertical            #\n");
		printf("# 11. Filtro Prewitt Horizontal        #\n");
		printf("# 12. Filtro Prewitt Vertical          #\n");
		printf("#                                      #\n");
		printf("# 0. Salir                             #\n");
		printf("########################################\n\n");
		
		printf("Introduce la opcion deseada: ");
		scanf("%d", &opcion);
	} while (opcion < 0 || opcion > 13);


	//Se llama a un método u otro según el filtro para crear la matriz de convolución correspondiente.
	switch (opcion) {
	case 1:
		matrizFiltroMedia3x3();
		break;
	case 2:
		matrizFiltroPasoBajo3x3();
		break;
	case 3:
		matrizFiltroPasoAlto3x3();
		break;
	case 4:
		matrizFiltroPasoAlto5x5();
		break;
	case 5:
		matrizFiltroGaussiano3x3();
		break;
	case 6:
		matrizFiltroGaussiano5x5();
		break;
	case 7:
		matrizFiltroLaplaciano3x3();
		break;
	case 8:
		matrizFiltroLaplaciano5x5();
		break;
	case 9:
		matrizFiltroSobelHorizontal3x3();
		break;
	case 10:
		matrizFiltroSobelVertical3x3();
		break;
	case 11:
		matrizFiltroPrewittHorizontal3x3();
		break;
	case 12:
		matrizFiltroPrewittVertical3x3();
		break;
	default:
		return 0;
	}

	imagenSalida = imagenEntrada;


	//Cálculo de la convolución:
	double sumaConvolucion;

	for (int x = imagenEntrada.FirstRow(); x <= imagenEntrada.LastRow(); x++) {
		for (int y = imagenEntrada.FirstCol(); y <= imagenEntrada.LastCol(); y++) {

			sumaConvolucion = 0;
			
			for (int i = matrizConvolucion.FirstRow(); i <= matrizConvolucion.LastRow(); i++) {
				for (int j = matrizConvolucion.FirstCol(); j <= matrizConvolucion.LastRow(); j++) {
					
					sumaConvolucion += CalcularValorPixel(x, y, i, j, matrizConvolucion);
				}
			}
			imagenSalida(x, y) = sumaConvolucion;
		}
	}

	//Reajuste de valores menores de 0 o mayores de 255 para que estén dentro del rango (0,255):
	ReajusteValores();
	
	//Escritura de la imagen de salida:
	char nombreImagenSalida[80] = "";
	strcat(nombreImagenSalida, nombreSinFormato);
	strcat(nombreImagenSalida, nombreFiltro);

	imagenSalida.WriteBMP(nombreImagenSalida);
	printf("\nEl filtro se ha aplicado correctamente. Se ha creado una nueva imagen llamada '%s'.\n", nombreImagenSalida);

	system("Pause");
}


int CalcularValorPixel(int x, int y, int i, int j, C_Matrix matrizConvolucion) {

	// OJO:	No confundir x e y con xConv e yConv, todas actuan recorriendo la imagen de entrada pero x e y
	//		muestran el pixel de destino mientras que xConv e yConv muestran sobre que pixel se está realizando
	//		el calculo con la matriz de convolucion.

	int xConv = (x - (matrizConvolucion.LastRow() / 2)) + i;
	int yConv = (y - (matrizConvolucion.LastRow() / 2)) + j;
	
	//Si el pixel está fuera de la imagen (tratamiento de bordes):
	
	// 1.Esquina superior izquierda:
	if (xConv < imagenEntrada.FirstRow() && yConv < imagenEntrada.FirstCol())
		return (double)matrizConvolucion(i, j) * (double)imagenEntrada(imagenEntrada.FirstRow(), imagenEntrada.FirstCol());

	// 2.Esquina superior derecha:
	else if(xConv < imagenEntrada.FirstRow() && yConv > imagenEntrada.LastCol())
		return (double)matrizConvolucion(i, j) * (double)imagenEntrada(imagenEntrada.FirstRow(), imagenEntrada.LastCol());

	// 3.Esquina inferior izquierda:
	else if (xConv > imagenEntrada.LastRow() && yConv < imagenEntrada.FirstCol())
		return (double)matrizConvolucion(i, j) * (double)imagenEntrada(imagenEntrada.LastRow(), imagenEntrada.FirstCol());

	// 4.Esquina inferior derecha:
	else if (xConv > imagenEntrada.LastRow() && yConv > imagenEntrada.LastCol())
		return (double)matrizConvolucion(i, j) * (double)imagenEntrada(imagenEntrada.LastRow(), imagenEntrada.LastCol());

	// 5.Lado superior:
	else if (xConv < imagenEntrada.FirstRow())
		return (double)matrizConvolucion(i, j) * (double)imagenEntrada(imagenEntrada.FirstRow(), yConv);

	// 6.Lado izquierdo:
	else if (yConv < imagenEntrada.FirstCol())
		return (double)matrizConvolucion(i, j) * (double)imagenEntrada(xConv, imagenEntrada.FirstCol());

	// 7.Lado inferior:
	else if (xConv > imagenEntrada.LastRow())
		return (double)matrizConvolucion(i, j) * (double)imagenEntrada(imagenEntrada.LastRow(), yConv);

	// 8.Lado derecho:
	else if (yConv > imagenEntrada.LastCol())
		return (double)matrizConvolucion(i, j) * (double)imagenEntrada(xConv, imagenEntrada.LastCol());

	// 9.Por defecto sin tratamiento de bordes:
	return  (double)matrizConvolucion(i, j) *  (double)imagenEntrada(xConv, yConv);
}

void ReajusteValores() {

	for (int x = imagenSalida.FirstRow(); x <= imagenSalida.LastRow(); x++) {
		for (int y = imagenSalida.FirstCol(); y <= imagenSalida.LastCol(); y++) {
			if (imagenSalida(x, y) < 0)
				imagenSalida(x, y) = 0;
			else if (imagenSalida(x, y) > 255)
				imagenSalida(x, y) = 255;
		}
	}
}


void matrizFiltroLaplaciano5x5() {
	C_Matrix matriz(-2, 2, -2, 2, 0);
	matriz(-1, 0) = -2;
	matriz(0, -1) = -2;
	matriz(1, 0) = -2;
	matriz(0, 1) = -2;
	matriz(0, 0) = 17; //16
	matriz(-2, 0) = -1;
	matriz(-1, -1) = -1;
	matriz(0, -2) = -1;
	matriz(0, 2) = -1;
	matriz(1, -1) = -1;
	matriz(2, 0) = -1;
	matriz(-1, 1) = -1;
	matriz(1, 1) = -1;

	printf("\n\nHas seleccionado el Filtro Laplaciano (5x5), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_laplaciano_5x5.bmp");
}

void matrizFiltroPasoAlto3x3() {
	C_Matrix matriz(-1, 1, -1, 1, -1);
	matriz(0, 0) = 8; //9

	printf("\n\nHas seleccionado el Filtro Paso Alto (3x3), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_pasoAlto_3x3.bmp");
}

void matrizFiltroSobelHorizontal3x3() {
	C_Matrix matriz(-1, 1, -1, 1, 0);
	matriz(-1, -1) = -1;
	matriz(-1, 0) = -2;
	matriz(-1, 1) = -1;
	matriz(1, -1) = 1;
	matriz(1, 0) = 2;
	matriz(1, 1) = 1;

	printf("\n\nHas seleccionado el Filtro Sobel Horizontal (3x3), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_sobelHorizontal_3x3.bmp");
}

void matrizFiltroSobelVertical3x3() {
	C_Matrix matriz(-1, 1, -1, 1, 0);
	matriz(-1, -1) = -1;
	matriz(0, -1) = -2;
	matriz(1, -1) = -1;
	matriz(-1, 1) = 1;
	matriz(0, 1) = 2;
	matriz(1, 1) = 1;

	printf("\n\nHas seleccionado el Filtro Sobel Vertical (3x3), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_sobelVertical_3x3.bmp");
}

void matrizFiltroMedia3x3() {
	C_Matrix matriz(-1, 1, -1, 1, 0.11);

	printf("\n\nHas seleccionado el Filtro Media (3x3), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_media_3x3.bmp");
}

void matrizFiltroPasoBajo3x3() {
	C_Matrix matriz(-1, 1, -1, 1, 0.13);
	matriz(0, 0) = 0;

	printf("\n\nHas seleccionado el Filtro Paso Bajo (3x3), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_pasoBajo_3x3.bmp");
}

void matrizFiltroPasoAlto5x5() {
	C_Matrix matriz(-2, 2, -2, 2, -1);
	matriz(0, 0) = 24; //25

	printf("\n\nHas seleccionado el Filtro Paso Alto (5x5), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_pasoAlto_5x5.bmp");
}

void matrizFiltroGaussiano3x3() {
	C_Matrix matriz(-1, 1, -1, 1, 0.13);
	matriz(0, 0) = 0.25;
	matriz(-1, -1) = 0.063;
	matriz(-1, 1) = 0.063;
	matriz(1, -1) = 0.063;
	matriz(1, 1) = 0.063;

	printf("\n\nHas seleccionado el Filtro Gaussiano (3x3), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_gaussiano_3x3.bmp");
}

void matrizFiltroGaussiano5x5() {
	C_Matrix matriz(-2, 2, -2, 2, 0.015);
	matriz(0, 0) = 0.15;
	matriz(-2, -2) = 0.0037;
	matriz(-2, 2) = 0.0037;
	matriz(2, 2) = 0.0037;
	matriz(2, -2) = 0.0037;
	matriz(-2, 0) = 0.026;
	matriz(0, -2) = 0.026;
	matriz(2, 0) = 0.026;
	matriz(0, 2) = 0.026;
	matriz(-1, -1) = 0.059;
	matriz(-1, 1) = 0.059;
	matriz(1, 1) = 0.059;
	matriz(1, -1) = 0.059;
	matriz(-1, 0) = 0.095;
	matriz(0, -1) = 0.095;
	matriz(1, 0) = 0.095;
	matriz(0, 1) = 0.095;

	printf("\n\nHas seleccionado el Filtro Gaussiano (5x5), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_gaussiano_5x5.bmp");
}

void matrizFiltroLaplaciano3x3() {
	C_Matrix matriz(-1, 1, -1, 1, 1);
	matriz(0, 0) = -4; //-3
	matriz(-1, -1) = 0;
	matriz(-1, 1) = 0;
	matriz(1, -1) = 0;
	matriz(1, 1) = 0;

	printf("\n\nHas seleccionado el Filtro Laplaciano (3x3), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_laplaciano_3x3.bmp");
}

void matrizFiltroPrewittVertical3x3() {
	C_Matrix matriz(-1, 1, -1, 1, 0);
	matriz(-1, -1) = -1;
	matriz(0, -1) = -1;
	matriz(1, -1) = -1;
	matriz(-1, 1) = 1;
	matriz(0, 1) = 1;
	matriz(1, 1) = 1;

	printf("\n\nHas seleccionado el Filtro PrewittVertical (3x3), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_prewittVertical_3x3.bmp");
}

void matrizFiltroPrewittHorizontal3x3() {
	C_Matrix matriz(-1, 1, -1, 1, 0);
	matriz(-1, -1) = -1;
	matriz(-1, 0) = -1;
	matriz(-1, 1) = -1;
	matriz(1, -1) = 1;
	matriz(1, 0) = 1;
	matriz(1, 1) = 1;

	printf("\n\nHas seleccionado el Filtro PrewittHorizontal (3x3), esta es su matriz de convolucion:\n\n");
	matrizConvolucion = matriz;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	strcat(nombreFiltro, "_prewittHorizontal_3x3.bmp");
}