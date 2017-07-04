#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class visao{
	#define MAX_NUM_FIG_GEOMETRICAS 100//numero maximo de figuras que serao armazenadas e desenhadas
	
	#define CALIBRACAO_1_CIRCULO	150 //quanto menor o valor mais falsos circulos serao detectados 
	#define CALIBRACAO_2_CIRCULO	45	//quanto menor o valor mais falsos circulos serao detectados
	VideoCapture video;

	Mat img_inicial;
	Mat img_tratada;

	vector <Point> circulo_centro;
	vector <int> circulo_raio;
	int num_circulos;
	Point retangulo_centro;

	Mat frame, frameFiltered, frame32f;
	float media[9];
	Mat mask;


	void filtro_inicial();
	void achar_circulos();
	void desenhar_circulos();

	bool iniciar_video();

	public:
	visao();
	void run();
};
