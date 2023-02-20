#ifndef FACE_ALIGN_H_
#define FACE_ALIGN_H_

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
#define KEYPOINT 5
#define POINTDIMS 2

class FaceAlgin
{
public:
    FaceAlgin(){}
    // 行优先
    cv::Mat estimate(float* src, float* standard)
    {
        // 1. 构建Q矩阵
        calculate_Q(src);
        // 2. 构建S矩阵
        S = cv::Mat(KEYPOINT*POINTDIMS, 1, CV_32FC1, standard);

        // 3. 最小二乘法
        M = (Q.t() * Q).inv() * Q.t() * S;  // shape: [4, 1]
        float similarity_data[6] = {*M.ptr<float>(0, 0), *M.ptr<float>(1, 0), *M.ptr<float>(2, 0),
                                    -*M.ptr<float>(1, 0), *M.ptr<float>(0, 0), *M.ptr<float>(3, 0)}; // [2, 3]
        cv::Mat similarity_matrix = cv::Mat(2, 3, CV_32FC1, similarity_data);

        return similarity_matrix.clone();
    }
private:
    cv::Mat Q;
    cv::Mat S;
    cv::Mat M ;

    void calculate_Q(const float* data_)
    {
        Q = cv::Mat::zeros(10, 4, CV_32FC1);
        for ( int i=0; i<KEYPOINT; ++i)
        {
            float x = data_[i*2];
            float y = data_[i*2 + 1];
            float* Qi = Q.ptr<float>(2 * i + 0);
            Qi[0] = x;
            Qi[1] = y;
            Qi[2] = 1.f;
            Qi[3] = 0.f;

            float* Qj = Q.ptr<float>(2 * i + 1);
            Qj[0] = y;
            Qj[1] = -x;
            Qj[2] = 0.f;
            Qj[3] = 1.f;
        }
    }

};

#endif //FACE_ALIGN_H_