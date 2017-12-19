#include <cmath>
#include <iostream>
#include "frequency.hpp"
#include "exprtk.hpp"

#define M_2_PI 0.63661977236758134308
#define e 2.71828182845904523536
double Frequency::evaluate(double mag, double freq, double x, const std::string &str) {

	typedef exprtk::symbol_table<double> symbol_table_t;
	typedef exprtk::expression<double>     expression_t;
	typedef exprtk::parser<double>             parser_t;
	typedef exprtk::parser_error::type			error_t;

	std::string expression_str = str;

	symbol_table_t table;
	table.clear_variables();
	table.add_constants();
	table.add_constant("e", e);
	table.add_variable("mag", mag);
	table.add_variable("freq", freq);
	table.add_variable("x", x);

	expression_t expression;
	expression.register_symbol_table(table);

	parser_t parser;

	if (!parser.compile(expression_str, expression))
	{
		printf("Error: %s\tExpression: %s\n",
			parser.error().c_str(),
			expression_str.c_str());

		for (std::size_t i = 0; i < parser.error_count(); ++i)
		{
			const error_t error = parser.get_error(i);
			printf("Error: %02d Position: %02d Type: [%s] Msg: %s Expr: %s\n",
				static_cast<int>(i),
				static_cast<int>(error.token.position),
				exprtk::parser_error::to_str(error.mode).c_str(),
				error.diagnostic.c_str(),
				expression_str.c_str());
		}

		return 1.0;
	}
	return expression.value();
}

double Frequency::userDef(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res, const std::string &str) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;

	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = evaluate(magS.x, freq.x, x, str);
	density.y = evaluate(magS.y, freq.y, y, str);
	density.z = evaluate(magS.z, freq.z, z, str);

	return (density.x + density.y + density.z);
}

double Frequency::rect(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	
	// period of 6 --> 111.000.111.000
	density.x = magS.x * static_cast<double>((cords.x % 6) < 3 ? 1 : -1);
	density.y = magS.y * static_cast<double>((cords.y % 6) < 3 ? 1 : -1);
	density.z = magS.z * static_cast<double>((cords.z % 6) < 3 ? 1 : -1);

	return (density.x + density.y + density.z);
}

double Frequency::triangle(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;

	// period of 6, values from 0 to 3
	density.x = magS.x * static_cast<double>(abs((cords.x % 6) - 3));
	density.y = magS.y * static_cast<double>(abs((cords.y % 6) - 3));
	density.z = magS.z * static_cast<double>(abs((cords.z % 6) - 3));

	return (density.x + density.y + density.z) / 3.0;
}

double Frequency::sine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;
	
	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (sin(freq.x * x) + 1.0) * 0.5;
	density.y = magS.y * (sin(freq.y * y) + 1.0) * 0.5;
	density.z = magS.z * (sin(freq.z * z) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}

double Frequency::quadraticSine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;

	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (pow(sin(freq.x * x), 2) + 1.0) * 0.5;
	density.y = magS.y * (pow(sin(freq.y * y), 2) + 1.0) * 0.5;
	density.z = magS.z * (pow(sin(freq.z * z), 2) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}

double Frequency::sineExp(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;

	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (exp(sin(freq.x * x)) / exp(1) + 1.0) * 0.5;
	density.y = magS.y * (exp(sin(freq.y * y)) / exp(1) + 1.0) * 0.5;
	density.z = magS.z * (exp(sin(freq.z * z)) / exp(1) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}

double Frequency::cosine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;

	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (cos(freq.x * x) + 1.0) * 0.5;
	density.y = magS.y * (cos(freq.y * y) + 1.0) * 0.5;
	density.z = magS.z * (cos(freq.z * z) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}

double Frequency::quadraticCosine(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;

	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (pow(cos(freq.x * x), 2) + 1.0) * 0.5;
	density.y = magS.y * (pow(cos(freq.y * y), 2) + 1.0) * 0.5;
	density.z = magS.z * (pow(cos(freq.z * z), 2) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}

double Frequency::cosineExp(const Vec3<int> &cords, const Vec3<double> &mag, const Vec3<int> &freq, const Vec3<int> &res) {
	double x = cords.x / static_cast<double>(res.x) * M_2_PI;
	double y = cords.y / static_cast<double>(res.y) * M_2_PI;
	double z = cords.z / static_cast<double>(res.z) * M_2_PI;

	Vec3<double> magS = mag;
	if (magS.x + magS.y + magS.z > 1.0) magS /= (magS.x + magS.y + magS.z);

	Vec3<double> density;
	density.x = magS.x * (exp(cos(freq.x * x)) / exp(1) + 1.0) * 0.5;
	density.y = magS.y * (exp(cos(freq.y * y)) / exp(1) + 1.0) * 0.5;
	density.z = magS.z * (exp(cos(freq.z * z)) / exp(1) + 1.0) * 0.5;

	return (density.x + density.y + density.z);
}