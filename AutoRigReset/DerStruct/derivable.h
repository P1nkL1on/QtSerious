#ifndef DERSTRUCT_DERIVABLE_H
#define DERSTRUCT_DERIVABLE_H


namespace DerStruct{

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

#endif // DERSTRUCT_DERIVABLE_H
