#ifndef DERIVABLE_H
#define DERIVABLE_H


namespace DerivableStructures{

class Derivable
{
private:
    double value;
    double derive;
public:
    Derivable():value(0), derive(0){}
    Derivable(float val):value(val), derive(0){}
    Derivable(double val):value(val), derive(0){}
    Derivable(double val, double der):value(val), derive(der){}

    double getValue() const;
    double getDerive() const;
};
}

#endif // DERIVABLE_H
