//---------------------------------------------------------------//
// CVecteur.h
//
// Définition des classes CVecteur et de leurs fonctions liées.
//
// FredDec (http://freddec.free.fr)
//
//---------------------------------------------------------------//

#ifndef CVecteur_h
#define CVecteur_h



//---------------------------------------------------------------//
// Include
//---------------------------------------------------------------//

#include <math.h>



//---------------------------------------------------------------//
// CVecteur3
//
// -> Représente un vecteur avec 3 composantes
//---------------------------------------------------------------//
class CVecteur3
{
	// Attributs ------------------------------------------------//

	public:

		double m_composante[3];

	// Construction / Destruction -------------------------------//

	public:

		CVecteur3(void);
		CVecteur3(double x, double y, double z);
		~CVecteur3(void);

	// Méthodes -------------------------------------------------//

	public:

		void Definir(double x, double y, double z);
		double CalculerNorme(void);
		void Normaliser(void);

	// Opérateurs -----------------------------------------------//

	public:

		double& operator[](int i);

		CVecteur3& operator=(const CVecteur3& v);
		CVecteur3& operator+=(const CVecteur3& v);

		const CVecteur3 operator+(const CVecteur3& v) const;
		const CVecteur3 operator-(const CVecteur3& v) const;
		const CVecteur3 operator*(double d) const;
};



//---------------------------------------------------------------//
// Fonctions
//---------------------------------------------------------------//

CVecteur3 ProduitVectoriel(CVecteur3& v0, CVecteur3& v1);
CVecteur3 CalculerNormale(CVecteur3& p0, CVecteur3& p1, CVecteur3& p2);






//---------------------------------------------------------------//
// CVecteur2
//
// -> Représente un vecteur avec 2 composantes
//---------------------------------------------------------------//
class CVecteur2
{
	// Attributs ------------------------------------------------//

	public:

		double m_composante[2];

	// Construction / Destruction -------------------------------//

	public:

		CVecteur2(void);
		CVecteur2(double x, double y);
		~CVecteur2(void);

	// Méthodes -------------------------------------------------//

	public:

		void Definir(double x, double y);

	// Opérateurs -----------------------------------------------//

	public:

		double& operator[](int i);

		CVecteur2& operator=(const CVecteur2& v);
		CVecteur2& operator+=(const CVecteur2& v);

		const CVecteur2 operator+(const CVecteur2& v) const;
		const CVecteur2 operator-(const CVecteur2& v) const;
		const CVecteur2 operator*(double d) const;
};



#endif