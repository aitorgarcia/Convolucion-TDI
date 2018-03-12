#include <stdio.h>
#include <C_Image.hpp>
#include <C_Matrix.hpp>
#include <C_General.hpp>

C_Image imagenEntrada;
C_Image imagenSalida;

int CalcularValorPixel(int x, int y, int i, int j, C_Matrix matrizConvolucion);
void ReajusteValores();

int main(int argc, char **argv)
{
	printf("################################################################\n");
	printf("#                                                              #\n");
	printf("#   APLICACION DE FILTROS MEDIANTE LA TECNICA DE CONVOLUCION   #\n");
	printf("#                                                              #\n");
	printf("#                                          Aitor Garcia Luiz   #\n");
	printf("################################################################\n\n");

	//Lectura de la imagen de entrada:
	imagenEntrada.ReadBMP(argv[1]);

	//Creación matriz de convolución:
	C_Matrix matrizConvolucion(-1, 1, -1, 1, -1);
	matrizConvolucion(0, 0) = 9;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);

	/*C_Matrix matrizConvolucion(-2, 2, -2, 2, -1);
	matrizConvolucion(0, 0) = 9;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);*/

	imagenSalida = imagenEntrada;


	//Cálculo de la convolución:
	int sumaConvolucion;

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
	//imagenSalida.Convolution(imagenEntrada, matrizConvolucion);

	//Reajuste de valores menores de 0 o mayores de 255 para que estén dentro del rango (0,255):
	ReajusteValores();
	
	//Escritura de la imagen de salida:
	imagenSalida.WriteBMP("convolution_image.bmp");

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
		return (int)matrizConvolucion(i, j) * (int)imagenEntrada(imagenEntrada.FirstRow(), imagenEntrada.FirstCol());

	// 2.Esquina superior derecha:
	else if(xConv < imagenEntrada.FirstRow() && yConv > imagenEntrada.LastCol())
		return (int)matrizConvolucion(i, j) * (int)imagenEntrada(imagenEntrada.FirstRow(), imagenEntrada.LastCol());

	// 3.Esquina inferior izquierda:
	else if (xConv > imagenEntrada.LastRow() && yConv < imagenEntrada.FirstCol())
		return (int)matrizConvolucion(i, j) * (int)imagenEntrada(imagenEntrada.LastRow(), imagenEntrada.FirstCol());

	// 4.Esquina inferior derecha:
	else if (xConv > imagenEntrada.LastRow() && yConv > imagenEntrada.LastCol())
		return (int)matrizConvolucion(i, j) * (int)imagenEntrada(imagenEntrada.LastRow(), imagenEntrada.LastCol());

	// 5.Lado superior:
	else if (xConv < imagenEntrada.FirstRow())
		return (int)matrizConvolucion(i, j) * (int)imagenEntrada(imagenEntrada.FirstRow(), yConv);

	// 6.Lado izquierdo:
	else if (yConv < imagenEntrada.FirstCol())
		return (int)matrizConvolucion(i, j) * (int)imagenEntrada(xConv, imagenEntrada.FirstCol());

	// 7.Lado inferior:
	else if (xConv > imagenEntrada.LastRow())
		return (int)matrizConvolucion(i, j) * (int)imagenEntrada(imagenEntrada.LastRow(), yConv);

	// 8.Lado derecho:
	else if (yConv > imagenEntrada.LastCol())
		return (int)matrizConvolucion(i, j) * (int)imagenEntrada(xConv, imagenEntrada.LastCol());

	// 9.Por defecto sin tratamiento de bordes:
	return  (int)matrizConvolucion(i, j) *  (int)imagenEntrada(xConv, yConv);
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