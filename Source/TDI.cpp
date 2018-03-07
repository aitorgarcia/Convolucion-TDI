#include <stdio.h>
#include <C_Image.hpp>
#include <C_Matrix.hpp>
#include <C_General.hpp>

void Convolution();
void TratamientoBordes();

int main(int argc, char **argv)
{
	printf("################################################################\n");
	printf("#                                                              #\n");
	printf("#   APLICACION DE FILTROS MEDIANTE LA TECNICA DE CONVOLUCION   #\n");
	printf("#                                                              #\n");
	printf("#                                          Aitor Garcia Luiz   #\n");
	printf("################################################################\n\n");

	C_Image imagenEntrada;
	imagenEntrada.ReadBMP("Hercules_Gris.bmp");

	//C_Matrix matrizConvolucion(0,2,0,2,-1);
	//matrizConvolucion(1, 1) = 8;

	C_Matrix matrizConvolucion(-1, 1, -1, 1, -1);
	matrizConvolucion(0, 0) = 8;

	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);



	C_Image imagenSalida = imagenEntrada;
	//imagenSalida.Print(imagenSalida.ColN(), (int)C_DOUBLE_P_INF);


	double sumaConvolucion;
	int rango = matrizConvolucion.LastRow();


	for (int x = imagenEntrada.FirstRow()+1; x <= imagenEntrada.LastRow()-1; x++) {
		for (int y = imagenEntrada.FirstCol()+1; y <= imagenEntrada.LastCol()-1; y++) {

			sumaConvolucion = 0;
			
			for (int i = matrizConvolucion.FirstRow(); i <= matrizConvolucion.LastRow(); i++) {
				for (int j = matrizConvolucion.FirstRow(); j <= matrizConvolucion.LastRow(); j++) {

					int comienzo = matrizConvolucion.LastRow() / 2;
					int xCalc = (x - comienzo) + i;
					int yCalc = (y - comienzo) + j;

					sumaConvolucion = sumaConvolucion + (matrizConvolucion(i, j) *  imagenEntrada(xCalc, yCalc));
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


void TratamientoBordes() {

}