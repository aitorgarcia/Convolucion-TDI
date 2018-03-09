#include <stdio.h>
#include <C_Image.hpp>
#include <C_Matrix.hpp>
#include <C_General.hpp>

void Convolution();
int CalcularValorPixel(int x, int y, int i, int j, C_Matrix matrizConvolucion);

C_Image imagenEntrada;

int main(int argc, char **argv)
{
	printf("################################################################\n");
	printf("#                                                              #\n");
	printf("#   APLICACION DE FILTROS MEDIANTE LA TECNICA DE CONVOLUCION   #\n");
	printf("#                                                              #\n");
	printf("#                                          Aitor Garcia Luiz   #\n");
	printf("################################################################\n\n");

	imagenEntrada.ReadBMP("lena512.bmp");

	C_Matrix matrizConvolucion(-1, 1, -1, 1, -1);
	matrizConvolucion(0, 0) = 9;
	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);



	C_Image imagenSalida = imagenEntrada;
	//imagenSalida.Print(imagenSalida.ColN(), (int)C_DOUBLE_P_INF);

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



	//Usar el constructor de la libreria Matrix para crear la matriz de convolución y que tenga los indices -1,0,1
	//	MoveSubMat? para mover la matriz de convolucion
	//	Trunc? para llevar los valores al límite del rango

	
	//imagenSalida.Convolution(imagenEntrada, matrizConvolucion);

	
	imagenSalida.Trunc(0, 255);

	//Una vez hecha la convolucion hacer una comprobación del flag fail
	//Hay que tener en cuenta la paleta de colores cuando se guarde la imagen en el disco.
	
	imagenSalida.WriteBMP("Hercules_Gris3.bmp");

	system("Pause");
}

void Convolution(){
	
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
		return matrizConvolucion(i, j) * imagenEntrada(imagenEntrada.FirstRow(), imagenEntrada.FirstCol());

	// 2.Esquina superior derecha:
	else if(xConv < imagenEntrada.FirstRow() && yConv > imagenEntrada.LastCol())
		return matrizConvolucion(i, j) * imagenEntrada(imagenEntrada.FirstRow(), imagenEntrada.LastCol());

	// 3.Esquina inferior izquierda:
	else if (xConv > imagenEntrada.LastRow() && yConv < imagenEntrada.FirstCol())
		return matrizConvolucion(i, j) * imagenEntrada(imagenEntrada.LastRow(), imagenEntrada.FirstCol());

	// 4.Esquina inferior derecha:
	else if (xConv > imagenEntrada.LastRow() && yConv > imagenEntrada.LastCol())
		return matrizConvolucion(i, j) * imagenEntrada(imagenEntrada.LastRow(), imagenEntrada.LastCol());

	// 5.Lado superior:
	else if (xConv < imagenEntrada.FirstRow())
		return matrizConvolucion(i, j) * imagenEntrada(imagenEntrada.FirstRow(), yConv);

	// 6.Lado izquierdo:
	else if (yConv < imagenEntrada.FirstCol())
		return matrizConvolucion(i, j) * imagenEntrada(xConv, imagenEntrada.FirstCol());

	// 7.Lado inferior:
	else if (xConv > imagenEntrada.LastRow())
		return matrizConvolucion(i, j) * imagenEntrada(imagenEntrada.LastRow(), yConv);

	// 8.Lado derecho:
	else if (yConv > imagenEntrada.LastCol())
		return matrizConvolucion(i, j) * imagenEntrada(xConv, imagenEntrada.LastCol());

	// 9.Por defecto sin tratamiento de bordes:
	return  matrizConvolucion(i, j) *  imagenEntrada(xConv, yConv);
}

