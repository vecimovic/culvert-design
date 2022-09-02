#include <iostream>
#include <math.h>
#include <tuple>
#include "culverts.h"

using namespace culverts;


std::tuple<double, std::string> calculate_HW(Culvert &culvert)
{

    double HW;
    std::string control_type;
    culvert.calculate_dc();
    double HWI = culvert.calculate_HWI();
    double HWO = culvert.calculate_HWO();

    if(HWI > HWO){
        HW = HWI;
        control_type = "INLET";
    }else{
        HW = HWO;
        control_type = "OUTLET";
    }

    return std::make_tuple(HW, control_type);
}


int main()
{
    int culvert_type, entrance_type;
    double Q, D, L, So, TW, ke, B, HW;
    std::string control_type;
    Culvert *culvert;
    bool iterate = 1;

    std::cout << "CULVERT DESIGN APPLICATION" << std::endl;
    std::cout << "Choose type of culvert:" << std::endl;
    std::cout << "1 - BOX CULVERT" << std::endl;
    std::cout << "2 - CIRCULAR CULVERT" << std::endl;
    std::cin >> culvert_type;

    while(culvert_type != 1 && culvert_type != 2)
    {
        std::cout << "WRONG INPUT! Try again." << std::endl;
        std::cin >> culvert_type;
    }

    std::cout << "Choose entrance type:" << std::endl;

    if(culvert_type == 1)
    {
        std::cout << "1 - Wingwall Flare 30 to 75 degrees" << std::endl;
        std::cout << "2 - Wingwall Flare 90 to 15 degrees" << std::endl;
        std::cout << "3 - Wingwall Flare 0 degrees" << std::endl;
    }

    if(culvert_type == 2)
    {
        std::cout << "1 - Square edge with headwall" << std::endl;
        std::cout << "2 - Groove end with headwall" << std::endl;
        std::cout << "3 - Groove end projecting" << std::endl;
    }

    std::cin >> entrance_type;

    while(entrance_type < 1 || entrance_type > 3)
    {
        std::cout << "WRONG INPUT! Try again." << std::endl;
        std::cin >> entrance_type;
    }

    std::cout << "Enter total flow, Q(m^3/s):" << std::endl;
    std::cin >> Q;

    while(Q < 0)
    {
        std::cout << "Value must be positive!Try again." << std::endl;
        std::cin >> Q;
    }

    std::cout << "Enter culvert diameter, D(m):" << std::endl;
    std::cin >> D;

    while(D < 0)
    {
        std::cout << "Value must be positive!Try again." << std::endl;
        std::cin >> D;
    }

    std::cout << "Enter culvert length, L(m):" << std::endl;
    std::cin >> L;

    while(L < 0)
    {
        std::cout << "Value must be positive!Try again." << std::endl;
        std::cin >> L;
    }

    std::cout << "Enter slope, So(m/m):" << std::endl;
    std::cin >> So;

    while(So < 0)
    {
        std::cout << "Value must be positive!Try again." << std::endl;
        std::cin >> So;
    }

    std::cout << "Enter tailwater depth, TW(m):" << std::endl;
    std::cin >> TW;

    while(TW < 0)
    {
        std::cout << "Value must be positive!Try again." << std::endl;
        std::cin >> TW;
    }

    std::cout << "Enter minor loss coefficient for pipe inlet, ke:" << std::endl;
    std::cin >> ke;

    while(ke < 0)
    {
        std::cout << "Value must be positive!Try again." << std::endl;
        std::cin >> ke;
    }

    if(culvert_type == 1){
        std::cout << "Enter culvert width, B(m):" << std::endl;
        std::cin >> B;

        while(B < 0)
        {
            std::cout << "Value must be positive!Try again." << std::endl;
            std::cin >> B;
        }

        BoxCulvert box_culvert(Q,D,L,So,TW,ke,B,entrance_type);
        culvert = &box_culvert;

    }else{

        CircularCulvert circular_culvert(Q,D,L,So,TW,ke,entrance_type);
        culvert = &circular_culvert;
    }

    tie(HW, control_type) = calculate_HW(*culvert);
    std::cout << "Headwater depth: "<< HW << ", control type: " << control_type << std::endl;

    double HWmax = 1.5 * D;

    if(HW > HWmax)
    {
        while(iterate)
        {
            std::cout << "HW > HWmax(" << HWmax <<"), you need to choose larger diameter D(m)" << std::endl;
            std::cin >> D;

            while(D < 0)
            {
                std::cout << "Value must be positive!Try again." << std::endl;
                std::cin >> D;
            }
            culvert->setD(D);
            tie(HW, control_type) = calculate_HW(*culvert);
            std::cout << "Headwater depth: "<< HW << ", control type: " << control_type << std::endl;

            if(HW <= HWmax)
            {
                 iterate = 0;
            }

        }

    }

    return 0;
}
