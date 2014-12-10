#ifndef GADGET_H
#define GADGET_H

class eGadget
{
protected:
    eGadget();
    virtual ~eGadget();

private:
    eGadget(const eGadget& aOther);
    eGadget& operator=(const eGadget& aOther);
};

#endif // GADGET_H
