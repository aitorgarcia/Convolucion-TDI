#include <stdio.h>
#include <C_Image.hpp>
#include <C_Matrix.hpp>
#include <C_General.hpp>

void Convolution();

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


	C_Matrix matrizConvolucion(-1,1,-1,1,-1);
	matrizConvolucion(0, 0) = 8;

	matrizConvolucion.Print(matrizConvolucion.ColN(), (int)C_DOUBLE_P_INF);




	int sumaConvolucion;


	C_Image imagenSalida;
	imagenSalida = imagenEntrada;

	
	printf("\n\nAplicando cambios...\n");

	int imagenInicio = imagenEntrada.FirstRow();
	int imagenFinal = imagenEntrada.LastRow();
	int imagenInicio2 = imagenEntrada.FirstCol();
	int imagenFinal2= imagenEntrada.LastCol();

	int imagenInicio3 = matrizConvolucion.FirstRow();
	int imagenFinal3 = matrizConvolucion.LastRow();
	int imagenInicio4 = matrizConvolucion.FirstCol();
	int imagenFinal4 = matrizConvolucion.LastCol();


	printf("Entra\n");
	for (int i = imagenInicio; i <= imagenFinal; i++) {
		for (int j = imagenInicio2; j <= imagenFinal2; j++) {
			printf("Entra imagen\n");
			sumaConvolucion = 0;
			for (int x = imagenInicio3; x <= imagenFinal3; x++) {
				for (int y = imagenInicio4; y <= imagenFinal4; y++) {
					printf("Entra matriz\n");
					sumaConvolucion = sumaConvolucion + (int)((int)imagenEntrada(i, j) * (int)matrizConvolucion(x, y));
					printf("Sale matriz\n");
				}
			}
			printf("Sale imagen\n");
			imagenSalida(i, j) = sumaConvolucion;
			printf("Suma\n");
		}
	}
	printf("Cambios aplicados.\n\n");
	

	

		


	//Usar el constructor de la libreria Matrix para crear la matriz de convolución y que tenga los indices -1,0,1
	//	MoveSubMat? para mover la matriz de convolucion
	//	Trunc? para llevar los valores al límite del rango
	
	imagenSalida.Trunc(0, 255);

	//Una vez hecha la convolucion hacer una comprobación del flag fail



	//Hay que tener en cuenta la paleta de colores cuando se guarde la imagen en el disco.
	
	
	imagenSalida.WriteBMP("Hercules_Gris2.bmp");


	system("Pause");
}


/*
void Convolution(){
	
}
*/