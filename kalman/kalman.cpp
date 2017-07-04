// Use Kalman Filter to model particle in circular trajectory.
//
#include "cv.h"
#include "highgui.h"
//#include "opencv/opencv_extra/learning_opencv_v2/cvx_defs.h"
#define phi2xy(mat)	cvPoint( cvRound(img->width/2 + img->width/3*cos(mat->data.fl[0])),cvRound( img->height/2 - img->width/3*sin(mat->data.fl[0])) )

#define CVX_RED		CV_RGB(0xff,0x00,0x00)
#define CVX_GREEN	CV_RGB(0x00,0xff,0x00)
#define CVX_BLUE	CV_RGB(0x00,0x00,0xff)

#define CVX_CYAN	CV_RGB(0x00,0xff,0xff)
#define CVX_MAGENTA	CV_RGB(0xff,0x00,0xff)
#define CVX_YELLOW	CV_RGB(0xff,0xff,0x00)

#define CVX_WHITE	CV_RGB(0xff,0xff,0xff)
#define CVX_BLACK	CV_RGB(0x00,0x00,0x00)
#define CVX_GRAY50	CV_RGB(0x88,0x88,0x88)

int main(int argc, char** argv) {
	// Initialize, create Kalman Filter object, window, random number
	// generator etc.
	//
	cvNamedWindow( "Kalman", 1 );
	CvRandState rng;
	cvRandInit( &rng, 0, 1, -1, CV_RAND_UNI );
	IplImage* img = cvCreateImage( cvSize(500,500), 8, 3 );
	CvKalman* kalman = cvCreateKalman( 2, 1, 0 );

	// state is (phi, delta_phi) - angle and angular velocity
	// Initialize with random guess.
	//
	CvMat* x_k = cvCreateMat( 2, 1, CV_32FC1 );
	cvRandSetRange( &rng, 0, 0.1, 0 );
	rng.disttype = CV_RAND_NORMAL;
	cvRand( &rng, x_k );
	// process noise
	//
	CvMat* w_k = cvCreateMat( 2, 1, CV_32FC1 );
	// measurements, only one parameter for angle
	//
	CvMat* z_k = cvCreateMat( 1, 1, CV_32FC1 );
	cvZero( z_k );
	// Transition matrix ‘F’ describes relationship between
	// model parameters at step k and at step k+1 (this is
	// the “dynamics” in our model)
	//
	const float F[] = { 1, 1, 0, 1 };
	memcpy( kalman->transition_matrix->data.fl, F, sizeof(F));
	// Initialize other Kalman filter parameters.
	//
	cvSetIdentity( kalman->measurement_matrix,cvRealScalar(1) );
	cvSetIdentity( kalman->process_noise_cov, cvRealScalar(1e-5) );
	cvSetIdentity( kalman->measurement_noise_cov, cvRealScalar(1e-1) );
	cvSetIdentity( kalman->error_cov_post, cvRealScalar(1));
	
	
	
	const CvMat* y_k;	
	// choose random initial state
	//
	cvRand( &rng, kalman->state_post );
	while( 1 ) {

		// predict point position
		y_k = cvKalmanPredict( kalman, 0 );
		// generate measurement (z_k)
		//
		cvRandSetRange(
			&rng,
			0,
			sqrt(kalman->measurement_noise_cov->data.fl[0]),
			0
		);
		cvRand( &rng, z_k );
		cvMatMulAdd( kalman->measurement_matrix, x_k, z_k, z_k );
		// plot points (eg convert to planar coordinates and draw)
		//
		cvZero( img );
		cvCircle( img, phi2xy(z_k), 4, CVX_YELLOW ); // observed state
		cvCircle( img, phi2xy(y_k), 4, CVX_WHITE, 2 ); // “predicted” state
		cvCircle( img, phi2xy(x_k), 4, CVX_RED );
		// real state
		cvShowImage( "Kalman", img );
		// adjust Kalman filter state
		//
		cvKalmanCorrect( kalman, z_k );
		// Apply the transition matrix ‘F’ (e.g., step time forward)
		// and also apply the “process” noise w_k.
		//
		cvRandSetRange(
			&rng,
			0,
			sqrt(kalman->process_noise_cov->data.fl[0]),
			0
		);
		cvRand( &rng, w_k );
		cvMatMulAdd( kalman->transition_matrix, x_k, w_k, x_k );
		// exit if user hits ‘Esc’
		if( cvWaitKey( 100 ) == 27 ) break;
	}
	return 0;
}
