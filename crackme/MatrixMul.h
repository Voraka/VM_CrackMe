#pragma once
#include <string>
#include<Eigen/dense>
using namespace Eigen;


class CMatrixMul 
{
public:
	CMatrixMul();
	~CMatrixMul();
	bool MMulEqual(std::string strIn);
	typedef Matrix<int, Dynamic, Dynamic> MatrixDI;

protected:
	// 两矩阵相异或（只适用于同型矩阵）
	void MXor(const MatrixDI& A, const MatrixDI& B, MatrixDI& C);

private:
	typedef Matrix<int, Dynamic, Dynamic> MatrixDI;
	MatrixDI A, B, D;
};

