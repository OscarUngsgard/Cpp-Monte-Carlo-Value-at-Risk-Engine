#pragma once
#define PAYOFF_H
//Can add any payoff for usage in a Monte-Carlo or Binomial Tree valuation.
class PayOff
{
public:
	PayOff() {};
	virtual double operator()(double Spot) const = 0;
	virtual ~PayOff() {}
	virtual PayOff* clone() const = 0;
private:
};

class PayOffStock : public PayOff
{
public:
	PayOffStock();
	virtual double operator()(double Spot) const;
	virtual ~PayOffStock() {}
	virtual PayOffStock* clone() const;
private:
};

class PayOffCall : public PayOff
{
public:
	PayOffCall(double Strike_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffCall() {}
	virtual PayOffCall* clone() const;
private:
	double Strike;
};

class PayOffPut : public PayOff
{
public:
	PayOffPut(double Strike_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffPut() {}
	virtual PayOffPut* clone() const;
private:
	double Strike;
};

class PayOffForward : public PayOff
{
public:
	PayOffForward(double Strike_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffForward() {}
	virtual PayOff* clone() const;
private:
	double Strike;
};