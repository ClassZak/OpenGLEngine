#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <string>


#ifdef _WIN32
#include <windows.h>
#endif




//#define MATRICIES_STDOUT




std::set<std::set<unsigned int>>getNumbersCombinations
(const unsigned int lastNumber,const unsigned int numbersGroup)
{
	
	std::vector<unsigned int>group;
	for(unsigned int i=0;i<lastNumber;++i)
		group.push_back(0);
	
	std::set<std::set<unsigned int> >allCombinations;
	std::set<unsigned int> currCombination;
	
	
	bool combinationSuccess=false;
	
	while(!combinationSuccess)
	{
		if(group.at(numbersGroup-1)<lastNumber)
			for(unsigned int j=0;j<numbersGroup;++j)
				currCombination.insert(group.at(j));
		
		if(currCombination.size()==numbersGroup)
		{
			allCombinations.insert(currCombination);
			currCombination.clear();
		}
		
		
		++group.at(0);
		
		
		for(unsigned int k=0;k<numbersGroup;++k)
		{
			if(group.at(k)>lastNumber-1)
			{
				if(k!=numbersGroup-1)
				{
					group.at(k)=0;
					++group.at(k+1);
				}
				else
				{
					if(std::all_of(currCombination.begin(),currCombination.end(),
						[lastNumber](unsigned int a)->bool
						{
							return a==lastNumber;
						}
					))
					combinationSuccess=true;
				}
			}
		}
		if(combinationSuccess)
			break;
		
		currCombination.clear();
	}
	return allCombinations;
}



class Matrix
{
	friend std::ostream& operator<<(std::ostream &os,const Matrix& M);//Вывод
	friend std::istream& operator>>(std::istream &os,Matrix& M);//ввод
public:
	Matrix();
	Matrix(const unsigned int m,const unsigned int n);//создание и укащзание размеров (m-число строк)
	Matrix(const Matrix& other);//Создание копии другой матрицы
	~Matrix();
	
	Matrix&operator=(const Matrix& other);//операция присваивания одной матрицы дрпугой
	Matrix&operator=(std::initializer_list<float> list);//это из примера
	Matrix&operator=(const std::vector<float>&array);//копирование вектора
	Matrix&operator*=(const float k);//умножение на число
	Matrix&operator*=(const Matrix& other);//умножение матриц
	Matrix&operator+=(const Matrix& other);//сложение матриц
	Matrix&operator-=(const Matrix& other);
	Matrix operator*(const float k);
	Matrix operator*(const Matrix& other);//тут то же самое, но без дополнительного самоприсвоения
	Matrix operator+(const Matrix& other);
	Matrix operator-(const Matrix& other);
	
	bool operator!=(const Matrix& other);//отношения
	bool operator==(const Matrix& other);
	
	
	float& operator()(const unsigned int i, const unsigned int j)const;
	
	
	
	void setElement(const unsigned int i, const unsigned int j, const float number);//задать элемент
	float getElement(const unsigned int i, const unsigned int j)const;//получить элемент
	void resetArray(const unsigned int m, const unsigned int n);//очистить матрицу
	void copyMatrixFromArray(const float* array,const unsigned int length);//копировать с массива
	void copyMatrixFromArray(const std::vector<float>&array);// копировать с вектора
	
	unsigned int getRows()const;//число строк
	unsigned int getColumns()const;//столбцов
private:
	float** array;
	unsigned int n;
	unsigned int m;
	
	void deleteArray();
public:
	static bool isDegenerate(const Matrix& matrix);
	static bool equalSizes(const Matrix& first, const Matrix& second);//проверка размеров
	static bool areConsistently(const Matrix& first, const Matrix& second);//согласованность 2 матриц
	static bool isSquare(const Matrix& matrix);//проверка на квадратность
	static bool isIdentity(const Matrix& matrix);//единичная или нет
	static Matrix createIdentityMatrix(const unsigned int order);//создать единичную матрицу
	static Matrix doTransponation(const Matrix& matrix);//сделать транспонирование
	static Matrix getMinor(const Matrix& matrix, const unsigned int i=0, const unsigned int j=0);//получить минор
	static Matrix getInverse(const Matrix& matrix);//обратная матрица
	static float getDeterminant(const Matrix& matrix);//определитель
	static float getAlgebraicComplement(const Matrix& matrix, const unsigned int i=0, const unsigned int j=0);
	//алгебраическое дополнение
	static unsigned int getRank(const Matrix& matrix);//получить ранг
	

};


Matrix solveSystemOfEquations(Matrix& A, Matrix& B)
{
	if (!Matrix::isSquare(A))
		throw std::runtime_error("Матрица коэффициентов должна быть квадратной");

	if (B.getColumns() != 1)
		throw std::runtime_error("Матрица свободных коэффициентов должна быть столбцом (n x 1)");

	if (A.getRows() != B.getRows())
		throw std::runtime_error("Число уравнений должно совпадать с числом неизвестных");

	const unsigned int n = A.getRows();
	const double eps = 1e-9;

	// Расширенная матрица [A | B] размера n x (n+1)
	std::vector<std::vector<double>> aug(n, std::vector<double>(n + 1));
	for (unsigned int i = 0; i < n; ++i) {
		for (unsigned int j = 0; j < n; ++j)
			aug[i][j] = A.getElement(i, j);
		aug[i][n] = B.getElement(i, 0);
	}

	// Прямой ход метода Гаусса с выбором главного элемента
	unsigned int rank = 0;
	std::vector<unsigned int> where(n, std::numeric_limits<unsigned int>::max());

	for (unsigned int col = 0, row = 0; col < n && row < n; ++col) {
		// Ищем строку с максимальным по модулю элементом в текущем столбце
		unsigned int sel = row;
		for (unsigned int i = row; i < n; ++i)
			if (std::abs(aug[i][col]) > std::abs(aug[sel][col]))
				sel = i;

		if (std::abs(aug[sel][col]) < eps)
			continue; // нет ведущего элемента в этом столбце

		std::swap(aug[row], aug[sel]);
		where[col] = row;

		// Нормируем строку
		double div = aug[row][col];
		for (unsigned int j = col; j <= n; ++j)
			aug[row][j] /= div;

		// Вычитаем из остальных строк
		for (unsigned int i = 0; i < n; ++i) {
			if (i != row && std::abs(aug[i][col]) > eps) {
				double factor = aug[i][col];
				for (unsigned int j = col; j <= n; ++j)
					aug[i][j] -= factor * aug[row][j];
			}
		}
		++row;
		++rank;
	}

	// Проверка на пустое множество решений ----------
	for (unsigned int i = rank; i < n; ++i) {
		double sum = 0;
		for (unsigned int j = 0; j < n; ++j)
			sum += std::abs(aug[i][j]);
		if (sum < eps && std::abs(aug[i][n]) > eps) {
			std::string msg = "Система несовместна (пустое множество решений).\n";
			msg += "Причина: rang(A) < rang([A|B]). После приведения к ступенчатому виду ";
			msg += "появилась строка вида 0 = b, где b != 0.\n";
			throw std::runtime_error(msg);
		}
	}

	// Проверка на бесконечное множество решений ----------
	if (rank < n) {
		std::stringstream ss;
		ss << "Система совместна, но имеет бесконечно много решений.\n";
		ss << "Причина: rang(A) = rang([A|B]) = " << rank << " < " << n << ".\n";
		ss << "Свободных переменных: " << (n - rank) << ".\n";
		ss << "Выразим базисные переменные через свободные:\n";

		std::vector<bool> freeVar(n, true);
		for (unsigned int col = 0; col < n; ++col)
			if (where[col] != std::numeric_limits<unsigned int>::max())
				freeVar[col] = false;

		for (unsigned int col = 0; col < n; ++col) {
			if (!freeVar[col]) {
				unsigned int row = where[col];
				ss << "x" << (col + 1) << " = " << aug[row][n];
				for (unsigned int j = 0; j < n; ++j) {
					if (freeVar[j]) {
						double coeff = -aug[row][j];
						if (std::abs(coeff) > eps)
							ss << " + (" << coeff << ")*t" << (j + 1);
					}
				}
				ss << "\n";
			}
		}
		ss << "где t1, t2, ... — произвольные временные переменные.\n";
		throw std::runtime_error(ss.str());
	}

	// Проверка на единственное решение ----------
	Matrix result(n, 1);
	for (unsigned int i = 0; i < n; ++i)
		result.setElement(i, 0, aug[where[i]][n]);

#ifdef MATRICIES_STDOUT
	std::cout << "Система имеет единственное решение.\n";
	std::cout << "Причина: rang(A) = rang([A|B]) = n = " << n << ".\n";
	std::cout << "Определитель матрицы A не равен нулю, поэтому обратная матрица существует.\n";
#endif

	return result;
}

Matrix::Matrix()
{
	this->array=nullptr;
	this->m=this->n=0;
}
Matrix::Matrix(const unsigned int m, const unsigned int n)
{
	this->array=nullptr;
	this->m=m;
	this->n=n;
	resetArray(m,n);
}
Matrix::Matrix(const Matrix& other)
{
	this->array=nullptr;
	if(*this!=other)
	*this=other;
}
Matrix::~Matrix()
{
	deleteArray();
}
//Operators
Matrix& Matrix::operator=(const Matrix& other)
{
	if((this->n!=other.n)or(this->m!=other.m))
	{
		this->deleteArray();
		this->resetArray(other.m,other.n);
	}
	
	for(unsigned int i=0;i<this->m;++i)
		for(unsigned int j=0;j<this->n;++j)
			this->array[i][j]=other.array[i][j];
	
	return *this;
}
Matrix& Matrix::operator=(const std::vector<float>&array)
{
	if(this->m*this->n!=array.size())
		throw std::runtime_error("Uncorrect array to copy into matrix");
	
	for(unsigned int i=0;i<this->m;++i)
		for(unsigned int j=0;j<this->n;++j)
			this->array[i][j]=array.at(n*i+j);
	
	return *this;
}

Matrix&Matrix::operator=(std::initializer_list<float> list)
{
	if(list.size()!=this->n*this->m)
		throw std::runtime_error("Uncorrect array to copy into matrix");
	
	for(std::size_t i=0;i<this->m;++i)
		for(std::size_t j=0;j<this->n;++j)
			this->array[i][j]=*(list.begin()+n*i+j);
	
	return *this;
}


Matrix& Matrix::operator*=(const float k)
{
#ifdef MATRICIES_STDOUT
	std::cout<<"Умножим матрицу "<<std::endl<<*this<<std::endl<<"на число"<<k<<':'<<std::endl;
#endif
	for(unsigned int i=0;i<m;++i)
		for(unsigned int j=0;j<n;++j)
			this->array[i][j]*=k;

	return *this;
}
Matrix& Matrix::operator+=(const Matrix& other)
{
#ifdef MATRICIES_STDOUT
	std::cout<<"Сложим матрицу"<<std::endl<<*this<<std::endl<<"с матрицей"<<std::endl<<other<<std::endl;
#endif
	if(!Matrix::equalSizes(*this,other))
		throw std::runtime_error("Неверные размеры во время сложения");
	for(unsigned int i=0;i<this->m;++i)
		for(unsigned int j=0;j<this->n;++j)
			this->array[i][j]+=other.array[i][j];
	return *this;
}
Matrix& Matrix::operator-=(const Matrix& other)
{
#ifdef MATRICIES_STDOUT
	std::cout<<"Сложим из матрицы"<<std::endl<<*this<<std::endl<<"матрицу"<<std::endl<<other<<std::endl;
#endif
	if(!Matrix::equalSizes(*this,other))
		throw std::runtime_error("Неверные размеры во время вычитания");
	for(unsigned int i=0;i<this->m;++i)
		for(unsigned int j=0;j<this->n;++j)
			this->array[i][j]-=other.array[i][j];
	return *this;
}
Matrix& Matrix::operator*=(const Matrix& other)
{
	if(!Matrix::areConsistently(*this,other))
		throw std::runtime_error("Ошибка при умножении матриц. Матрицы должны быть согласованы");
	
	Matrix result=*this*other;
	*this=result;
	return *this;
}



Matrix Matrix::operator*(const Matrix& other)
{
	if(!Matrix::areConsistently(*this,other))
	throw std::runtime_error("Ошибка при умножении матриц. Матрицы должны быть согласованы");
		Matrix result(this->m,other.n);
#ifdef MATRICIES_STDOUT
	std::cout<<"Матрица как результат умножения должна иметь количество строк из первой матрицы и количество"<<
	" столбцов из второй матрицы"<<std::endl<<'('<<this->m<<"\t и \t"<<other.n<<')'<<std::endl;
#endif
	
	float element=0;
	for(unsigned int i=0;i<result.m;++i)
		for(unsigned int j=0;j<result.n;++j)
		{
			for(unsigned int k=0;k<this->n;++k)
				element+=this->array[i][k]*other.array[k][j];
			
			result.array[i][j]=element;
			element=0;
		}
	return result;
}
Matrix Matrix::operator*(const float k)
{
	Matrix result=*this;
	result*=k;
	return result;
}
Matrix Matrix::operator+(const Matrix& other)
{
	Matrix result=*this;
	result+=other;
	return result;
}
Matrix Matrix::operator-(const Matrix& other)
{
	Matrix result=*this;
	result-=other;
	return result;
}



bool Matrix::operator!=(const Matrix& other)
{
	if((this->n!=other.n)or(this->m!=other.m))
		return true;
	bool equal=true;
	
	for(unsigned int i=0;i<this->m;++i)
		for(unsigned int j=0;j<this->n;++j)
		{
			if(this->array[i][j]!=other.array[i][j])
			{
				equal=false;
				break;
			}
		}
	return !equal;
}
bool Matrix::operator==(const Matrix& other)
{
	if((this->n!=other.n)or(this->m!=other.m))
		return false;
	bool equal=true;
	
	for(unsigned int i=0;i<this->m;++i)
		for(unsigned int j=0;j<this->n;++j)
		{
			if(this->array[i][j]!=other.array[i][j])
			{
				equal=false;
				break;
			}
		}
	return equal;
}


float& Matrix::operator()(const unsigned int i,const unsigned int j)const
{
	if(!(i<m and j<n))
	throw std::runtime_error("Неверный индекс элемента матрицы");
	else
	return array[i][j];
}
//Stream operators
std::ostream& operator<<(std::ostream &os,const Matrix& M)
{
	for(unsigned int i=0;i<M.m;++i)
	{
		for(unsigned int j=0;j<M.n;++j)
			os<<M.array[i][j]<<'\t';
		os<<std::endl;
	}
	
	return os;
}
std::istream& operator>>(std::istream &os,Matrix& M)
{
	unsigned int m,n;
#ifdef MATRICIES_STDOUT
	std::cout<<"Введите размеры матрицы (m и n)"<<std::endl<<"m=";
#endif
	os>>m;
#ifdef MATRICIES_STDOUT
	std::cout<<"n=";
#endif
	os>>n;
	
	if(!m or !n)
		throw std::runtime_error("Wrong sizes");
	
	M.resetArray(m,n);
	
	float current;
#ifdef MATRICIES_STDOUT
	std::cout<<"Введите матрицу"<<std::endl;
#endif
	
	std::string line;
	//std::getline(os,line);
	std::getline(os,line);
	for(unsigned int i=0;i<m;++i)
	{
		std::string line;
		std::getline(os,line);
		std::stringstream ss(line);
		for(unsigned int j=0;j<n;++j)
		ss>>M(i,j);
	}
	
	
	return os;
}
//Static public methods
unsigned int Matrix::getRank(const Matrix& matrix)
{
#ifdef MATRICIES_STDOUT
	std::cout<<"Если найден опрделитель одной из минор n-го порядка, то ранг повышается"<<std::endl;
#endif
	unsigned int rank=0;
	bool haveNotOnlyNulls=true;
	for(unsigned int i=0;i<matrix.m;++i)
	{
		for(unsigned int j=0;j<matrix.n;++j)
		{
			if(matrix.array[i][j])
			{
				++rank;
				haveNotOnlyNulls=false;
				#ifdef MATRICIES_STDOUT
				std::cout<<"Найден не нулевой элемент - ранг больше 0"<<std::endl;
				#endif
				break;
			}
		}
		if(!haveNotOnlyNulls)
		break;
	}
	
	if(haveNotOnlyNulls)
	{
		#ifdef MATRICIES_STDOUT
		std::cout<<"Не найден не нулевой элемент - ранг равен 0"<<std::endl;
		#endif
		return rank;
	}
	
	haveNotOnlyNulls=true;

	#ifdef MATRICIES_STDOUT
	std::cout<<"Ранг матрицы каждый раз определяется всё большим числом, если одна из комбинаций строк и "<<
	"столбцов формирует минор с ненулевым определителем"<<std::endl;
	#endif
	
	while((matrix.n>=(rank+1))and(matrix.m>=(rank+1)))
	{
		haveNotOnlyNulls=false;
		std::set<std::set<unsigned int> >xPositions;xPositions=getNumbersCombinations(matrix.n,rank+1);
		std::set<std::set<unsigned int> >yPositions;yPositions=getNumbersCombinations(matrix.m,rank+1);
		
		std::vector<float> currArray;
		
		Matrix minor(rank+1,rank+1);
		currArray.reserve((rank+1)*(rank+1));
		
		for(std::set<std::set<unsigned int> >::iterator it=xPositions.begin();it!=xPositions.end();++it)
		{
			for(int i=0;i<matrix.m-rank;++i)
			{
				currArray.clear();
				for(int y=0;y!=rank+1;++y)
					for(std::set<unsigned int>::iterator xIt=it->begin();xIt!=it->end();++xIt)
					{
						currArray.push_back(matrix(y+i,*xIt));
					}
				
				

				#ifdef MATRICIES_STDOUT
				if(currArray.size()!=(rank+1)*(rank+1))
				{
					std::cout<<"Ошибка: разер скопированного массива:"<<currArray.size()<<std::endl;
					std::cout<<"Массив:";
					for(std::vector<float>::iterator itV=currArray.begin();itV!=currArray.end();++itV)
					std::cout<<*itV<<'\t';
					std::cout<<std::endl;
				}
				#endif
				
				
				minor=currArray;
				
				#ifdef MATRICIES_STDOUT
				std::cout<<"Текущий минор:"<<std::endl<<minor<<std::endl;
				#endif
				float det=Matrix::getDeterminant(minor);
				bool filled=bool(det!=0.f);
				#ifdef MATRICIES_STDOUT
				std::cout<<"Определитель:"<<det<<std::endl;
				#endif
				
				
				
				if(filled)
				{
					haveNotOnlyNulls=true;
					break;
				}
				if(haveNotOnlyNulls)
				break;
			}
			
			if(haveNotOnlyNulls)
			break;
		}
		if(haveNotOnlyNulls)
		{
			#ifdef MATRICIES_STDOUT
			std::cout<<"Найден минор с ненулевым определителем, значит, ранг больше, чем "<<rank<<std::endl;
			#endif
			++rank;
			continue;
		}
		
		
		
		for(std::set<std::set<unsigned int> >::iterator it=yPositions.begin();it!=yPositions.end();++it)
		{
			for(int i=0;i<matrix.n-rank;++i)
			{
				currArray.clear();
				for(std::set<unsigned int>::iterator yIt=it->begin();yIt!=it->end();++yIt)
					for(int x=0;x!=rank+1;++x)
						currArray.push_back(matrix(*yIt,x+i));
				
				#ifdef MATRICIES_STDOUT
				if(currArray.size()!=(rank+1)*(rank+1))
				{
					std::cout<<"Ошибка: разер скопированного массива:"<<currArray.size()<<std::endl;
					std::cout<<"Массив:";
					for(std::vector<float>::iterator itV=currArray.begin();itV!=currArray.end();++itV)
					std::cout<<*itV<<'\t';
					std::cout<<std::endl;
				}
				#endif
				
				
				minor=currArray;
				
				#ifdef MATRICIES_STDOUT
				std::cout<<"Текущий минор:"<<std::endl<<minor<<std::endl;
				#endif
				float det=Matrix::getDeterminant(minor);
				bool filled=bool(det!=0.f);
				#ifdef MATRICIES_STDOUT
				std::cout<<"Определитель:"<<det<<std::endl;
				#endif
				
				if(filled)
				{
					haveNotOnlyNulls=true;
					break;
				}
				
				if(haveNotOnlyNulls)
				break;
			}
			
			if(haveNotOnlyNulls)
			break;
		}
		if(haveNotOnlyNulls)
		{
			#ifdef MATRICIES_STDOUT
			std::cout<<"Найден минор с ненулевым определителем, значит, ранг больше, чем "<<rank<<std::endl;
			#endif
			++rank;
			continue;
		}
		else
		break;
	}
	return rank;
}
Matrix Matrix::getInverse(const Matrix& matrix)
{
	if(!Matrix::isSquare(matrix))
		throw std::runtime_error("Неудалось получить обратную матрицу из не квадратной");
	
	std::cout<<"проверим определитель матрицы"<<std::endl;
	float determinant=Matrix::getDeterminant(matrix);
	
	if(determinant==0.f)
		throw std::runtime_error("матрица вырождена, поэтому получить обратную матрицу невозможно");
	
	std::cout<<"Определитель матрицы равен "<<determinant<<std::endl;
	std::cout<<"Получим матрицу из алгебраических дополнений"<<std::endl;
	
	unsigned int order=matrix.getRows();
	std::vector<float> array;
	Matrix result(order,order);
	
	
	array.reserve(order*order);
	
	for(unsigned int i=0;i<order;++i)
		for(unsigned int j=0;j<order;++j)
			array.push_back(Matrix::getAlgebraicComplement(matrix,i,j));
	
	result.copyMatrixFromArray(array);
	
	std::cout<<"Матрица из определителей:"<<std::endl<<result<<std::endl;
	std::cout<<"Транспонируем матрицу из определителей:"<<std::endl;
	result=(Matrix)Matrix::doTransponation(result);
	
	std::cout<<"Умножим матрицу на число, обратное определителю"<<std::endl;
	result*=(1.f/(float)Matrix::getDeterminant(matrix));
	
	return result;
}
bool Matrix::equalSizes(const Matrix& first,const Matrix& second)
{
	return(first.n==second.n)and(first.m==second.m);
}
bool Matrix::areConsistently(const Matrix& first,const Matrix& second)
{
	std::cout<<"Матрицы согласованы, если число столбцов первой матрицы равно числу строк второй"<<std::endl;
	return first.n==second.m;
}
bool Matrix::isSquare(const Matrix& matrix)
{
	std::cout<<"Размеры матрицы:"<<std::endl<<"m="<<matrix.m<<std::endl<<"n="<<matrix.n<<std::endl;
	
	(matrix.n==matrix.m) ? 
	std::cout<<"Матрица квадтратная"<<std::endl :
	std::cout<<"Матрица не квадтратная"<<std::endl;
	
	return matrix.n==matrix.m;
}
bool Matrix::isIdentity(const Matrix& matrix)
{
	if(!Matrix::isSquare(matrix))
		return false;

	bool identity=true;
	for(unsigned int i=0;i<matrix.m;++i)
		for(unsigned int j=0;j<matrix.n;++j)
		{
			if(i==j)
			{
				if(matrix.array[i][j]!=1)
				{
					identity=false;
					break;
				}
			}
			else
			if(matrix.array[i][j])
			{
				identity=false;
				break;
			}
		}
	
	return identity;
}
bool Matrix::isDegenerate(const Matrix& matrix)
{
	std::cout<<"Матрица вырождена, если определитель равен 0"<<std::endl;
	float determinant=Matrix::getDeterminant(matrix);
	if(determinant>0 and determinant<0.0000001f)
		std::cout<<"Из-за неточности вычислений определитель близок к 0, но не равен ему"<<std::endl;
	return determinant==0.f;
}
Matrix Matrix::doTransponation(const Matrix& matrix)
{
	std::cout<<"Транспонирование - замена строк матрицы столбцами"<<std::endl;
	Matrix result(matrix.n,matrix.m);
	for(unsigned int i=0;i<matrix.m;++i)
		for(unsigned int j=0;j<matrix.n;++j)
			result.array[j][i]=matrix.array[i][j];
	
	return result;
}
Matrix Matrix::createIdentityMatrix(const unsigned int order)
{
	Matrix result(order,order);
	for(unsigned int i=0;i<order;++i)
		result.array[i][i]=1;
	
	return result;
}
float Matrix::getDeterminant(const Matrix& matrix)
{
	if(!Matrix::isSquare(matrix))
		throw std::runtime_error("Невозможно получить определитель из не квадтратной матрицы");
	
	#ifdef MATRICIES_STDOUT
	std::cout<<"Найдём определитель матрицы"<<std::endl<<matrix<<':'<<std::endl;
	#endif

	const unsigned int order=matrix.getRows();
	
	if(order==1)
		return matrix.getElement(0,0);
	if(order==2)
	{
		float determinant,sideDiagonal,mainDiagonal;
		
		mainDiagonal=matrix.getElement(0,0)*matrix.getElement(1,1);
		#ifdef MATRICIES_STDOUT
		std::cout<<"Главная диагональ="<<
		matrix.getElement(0,0)<<"\t*\t"<<matrix.getElement(1,1)<<"\t=\t"<<mainDiagonal<<std::endl;
		#endif
		
		sideDiagonal=matrix.getElement(1,0)*matrix.getElement(0,1);
		#ifdef MATRICIES_STDOUT
		std::cout<<"Побочная диагональ="<<
		matrix.getElement(1,0)<<"\t*\t"<<matrix.getElement(0,1)<<"\t=\t"<<sideDiagonal<<std::endl;
		#endif
		
		determinant=mainDiagonal-sideDiagonal;
		#ifdef MATRICIES_STDOUT
		std::cout<<"Определитель равен главная диагональ минус побочная:"<<std::endl;
		std::cout<<mainDiagonal<<"\t-\t"<<sideDiagonal<<"\t=\t"<<determinant<<std::endl;
		#endif
		
		return determinant;
	}
	else if(order==3)
	{
		float determinant,sideDiagonal,mainDiagonal,firstTriangle,secondTriangle,firstDeterminant;
		
		#ifdef MATRICIES_STDOUT
		std::cout<<"Найдём главную диагональ и два треугольника к ней"<<std::endl;
		#endif
		
		mainDiagonal=matrix(0,0)*matrix(1,1)*matrix(2,2);

		#ifdef MATRICIES_STDOUT
		std::cout<<"Главная диагональ="<<matrix(0,0)<<"\t*\t"<<
		matrix(1,1)<<"\t*\t"<<matrix(2,2)<<"\t=\t"<<mainDiagonal<<std::endl;
		#endif

		firstTriangle=matrix(0,1)*matrix(1,2)*matrix(2,0);
		#ifdef MATRICIES_STDOUT
		std::cout<<"Первый треугольник главной диагонали="<<matrix(0,1)<<"\t*\t"<<
		matrix(1,2)<<"\t*\t"<<matrix(2,0)<<"\t=\t"<<firstTriangle<<std::endl;
		#endif
		
		secondTriangle=matrix(1,0)*matrix(2,1)*matrix(0,2);
		#ifdef MATRICIES_STDOUT
		std::cout<<"Воторой треугольник главной диагонали="<<matrix(1,0)<<"\t*\t"<<
		matrix(2,1)<<"\t*\t"<<matrix(0,2)<<"\t=\t"<<secondTriangle<<std::endl;
		#endif
		
		
		firstDeterminant=mainDiagonal+firstTriangle+secondTriangle;
		#ifdef MATRICIES_STDOUT
		std::cout<<"Первая часть определителя (часть выражения):"<<firstDeterminant<<std::endl;
		#endif
		
		

		#ifdef MATRICIES_STDOUT
		std::cout<<"Найдём побочную диагональ и два треугольника к ней"<<std::endl;
		#endif
		
		mainDiagonal=matrix(0,2)*matrix(1,1)*matrix(2,0);
		#ifdef MATRICIES_STDOUT
		std::cout<<"Главная диагональ="<<matrix(0,2)<<"\t*\t"<<
		matrix(1,1)<<"\t*\t"<<matrix(2,0)<<"\t=\t"<<mainDiagonal<<std::endl;
		#endif
		
		firstTriangle=matrix(0,0)*matrix(1,2)*matrix(2,1);
		#ifdef MATRICIES_STDOUT
		std::cout<<"Первый треугольник побочной диагонали="<<matrix(0,0)<<"\t*\t"<<
		matrix(1,2)<<"\t*\t"<<matrix(2,1)<<"\t=\t"<<firstTriangle<<std::endl;
		#endif
		
		secondTriangle=matrix(1,0)*matrix(0,1)*matrix(2,2);
		#ifdef MATRICIES_STDOUT
		std::cout<<"Воторой треугольник побочной диагонали="<<matrix(1,0)<<"\t*\t"<<
		matrix(0,1)<<"\t*\t"<<matrix(2,2)<<"\t=\t"<<secondTriangle<<std::endl;
		#endif
		
		
		determinant=mainDiagonal+firstTriangle+secondTriangle;
		#ifdef MATRICIES_STDOUT
		std::cout<<"Вторая часть определителя (часть выражения):"<<determinant<<std::endl;
		
		std::cout<<"В итоге, определитель равен\t"<<firstDeterminant<<"\t-\t"<<determinant;
		determinant=firstDeterminant-determinant;
		std::cout<<"\t=\t"<<determinant<<std::endl;
		#endif
		
		return determinant;
	}
	else
	{
		#ifdef MATRICIES_STDOUT
		std::cout<<"Матрица имеет размеры больше 3, поэтомум воспользуемся свойством разложения определителя"<<
		std::endl<<"Сначла определитель равен 0"<<std::endl;
		#endif
		
		Matrix* minors=new Matrix[order];
		float determinant=0;
		for(unsigned int i=0;i<order;++i)
		{
			minors[i]=Matrix::getMinor(matrix,0,i);
			#ifdef MATRICIES_STDOUT
			std::cout<<"Найдём "<<i+1<<" минор:"<<minors[i]<<std::endl;
			std::cout<<"Прибавим (-1)^"<<i+2<<" * элемент "<<0<<'\t'<<i<<" * определитель миноры"<<i+1<<std::endl;
			std::cout<<"Получаем текущий результат:"<<
			Matrix::getDeterminant(minors[i])*std::pow(-1,i+2)*matrix.getElement(0,i)<<std::endl;
			#endif
			
			#ifdef MATRICIES_STDOUT
			std::cout<<"Прибавляем к нашему определителю, который сейчас равен "<<
			determinant<<", текущий результат"<<std::endl;
			#endif
			
			determinant+=Matrix::getDeterminant(minors[i])*std::pow(-1,i+2)*matrix.getElement(0,i);
			
			#ifdef MATRICIES_STDOUT
			std::cout<<"Теперь определитель равен "<<determinant<<std::endl;
			#endif
		}
		delete [] minors;
		
		#ifdef MATRICIES_STDOUT
		std::cout<<"В итоге определитель матрицы равен "<<determinant<<std::endl;
		#endif
		return determinant;
	}
}
Matrix Matrix::getMinor(const Matrix& matrix,const unsigned int i,const unsigned int j)
{
	if(!Matrix::isSquare(matrix))
		throw std::runtime_error("Невозможно получить минор из не квадратной матрицы");
	
	if(matrix.n==1)
		throw std::runtime_error("Невозможно получить минор из матрицы с одним элементом");
	
	if(matrix.n<=j and matrix.m<=i)
		throw std::runtime_error("Номера строки и столбца неверны");
	if(matrix.n<=j)
		throw std::runtime_error("Номер столбца неверен");
	if(matrix.m<=i)
		throw std::runtime_error("Номер строки неверен");
	
	const unsigned int order=matrix.getRows();
	Matrix mMatrix(order-1,order-1);
	
	std::vector<float> array;
	array.reserve((order-1)*(order-1));
	for(unsigned int i_=0;i_<order;++i_)
	{
		for(unsigned int j_=0;j_<order;++j_)
		{
			if((i==i_)or(j==j_))
				continue;
			array.push_back(matrix.getElement(i_,j_));
		}
	}
	Matrix matrixToDeterminant(order-1,order-1);
	matrixToDeterminant.copyMatrixFromArray(array);
	
	return matrixToDeterminant;
}
float Matrix::getAlgebraicComplement(const Matrix& matrix,const unsigned int i,const unsigned int j)
{
	if(!Matrix::isSquare(matrix))
		throw std::runtime_error("Матрица не квадратна для получения алгебраического дополнения");
	
	#ifdef MATRICIES_STDOUT
	std::cout<<"Алгебраическое дополнение - это определитель минора, умноженный на -1 в стпени номер строки плюс"<<
	" номер столбца"<<std::endl;
	#endif
	float complement=std::pow(-1,i+j)*Matrix::getDeterminant(Matrix::getMinor(matrix,i,j));
	std::cout<<"Дополнение: "<<complement<<std::endl;
	
	return complement;
}
//Public methods
void Matrix::setElement(const unsigned int i,const unsigned int j,const float number)
{
	if(i>=m or j>=n)
		throw std::runtime_error("Uncorrect index of element");
	
	this->array[i][j]=number;
}
float Matrix::getElement(const unsigned int i,const unsigned int j)const
{
	if(i>=m or j>=n)
		throw std::runtime_error("Uncorrect index of element");
	
	return this->array[i][j];
}
void Matrix::resetArray(const unsigned int m,const unsigned int n)
{
	deleteArray();
	this->n=n;
	this->m=m;
	
	array=new float*[m];
	for(unsigned int i=0;i<m;++i)
	{
		array[i]=new float[n];
		for(unsigned j=0;j<n;++j)
		array[i][j]=0;
	}
}
unsigned int Matrix::getRows()const
{
	return m;
}
unsigned int Matrix::getColumns()const
{
	return n;
}
void Matrix::copyMatrixFromArray(const float* array,const unsigned int length)
{
	if(this->m*this->n!=length)
		throw std::runtime_error("Uncorrect array to copy into matrix");
	
	for(unsigned int i=0;i<this->m;++i)
		for(unsigned int j=0;j<this->n;++j)
			this->array[i][j]=array[n*i+j];
}
void Matrix::copyMatrixFromArray(const std::vector<float>&array)
{
	if(this->m*this->n!=array.size())
		throw std::runtime_error("Uncorrect array to copy into matrix");
	
	for(unsigned int i=0;i<this->m;++i)
		for(unsigned int j=0;j<this->n;++j)
			this->array[i][j]=array.at(n*i+j);
}


void Matrix::deleteArray()
{
	if(array!=nullptr)
	{
		for(unsigned int i=0;i<m;++i)
			delete [] array[i];
		delete [] array;
		array = nullptr;
	}
}

