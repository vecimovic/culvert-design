#ifndef CULVERTS_H
#define CULVERTS_H

#endif // CULVERTS_H
#include <math.h>

namespace culverts
{
    class Culvert
    {
        protected:
            double Q;
            double D;
            double L;
            double So;
            double TW;
            double ke;
            int entrance_type;
            double dc;
        public:
            const double g = 9.81;
            const double PI = 3.14;
            const double n = 0.011;

            Culvert(double Q, double D, double L, double So, double TW, double ke, int entrance_type)
            {
                this->Q = Q;
                this->D = D;
                this->L = L;
                this->So = So;
                this->TW = TW;
                this->entrance_type = entrance_type;
            }

            virtual double calculate_HWI() = 0;
            virtual void calculate_dc() = 0;
            virtual double calculate_A() = 0;
            virtual double calculate_Rh() = 0;

            void setD(double D){
                this->D = D;
            }

            double calculate_fi()
            {
                return acos(1 - (2 * dc / D));
            }

            double calculate_v()
            {
                return Q / calculate_A();
            }

            double calculate_Sf()
            {
                return pow(n, 2) * pow(calculate_v(), 2) * pow(calculate_Rh(), 2);
            }

            double calculate_H()
            {
                double v_squared = pow(calculate_v(), 2);
                return v_squared / 2*g + calculate_Sf() * L + ke * v_squared / 2*g;

            }

            double calculate_h0()
            {
                double h0;
                if(TW >= D || (dc < D && (dc + D) / 2 > TW)){
                    h0 = TW;
                }else{
                    h0 = (dc + D) / 2;
                }
                return h0;
            }

            double calculate_HWO()
            {
                return calculate_h0() + calculate_H() - So * L;
            }

    };

    class BoxCulvert : public Culvert
    {
        public:
            double B;

            BoxCulvert(double Q, double D, double L, double So, double TW, double ke, double B, int entrance_type)
                : Culvert(Q,D,L,So,TW,ke,entrance_type)
            {
               this->B = B;
            }

            double calculate_HWI()
            {

                double HW = dc + pow(calculate_v(), 2) / 2*g;
                double HW1;
                if(HW / D < 1.35)
                {
                    HW1 = pow(Q / 0.544 * pow(g, 0.5) * B, 1.0 / 1.5);
                }else
                {
                    HW1 = pow(Q / 0.702 * pow(g, 0.5) * B * pow(D, 0.89), 1.0 / 0.61);
                }

                if(entrance_type == 2)
                {
                    return 1.09 * pow(HW1 / D, 0.99);
                }else if(entrance_type == 3)
                {
                    return 1.07 * pow(HW1 / D, 1.08);
                }

                return HW1;
            }

            void calculate_dc()
            {
                dc = 0.4672 * pow(Q / B, 0.667);
                if(dc >= D)
                {
                    dc = D;
                }

            }

            double calculate_A()
            {
                return B * D;
            }

            double calculate_Rh()
            {
                return B + 2 * D;
            }

    };

    class CircularCulvert: public Culvert
    {
        public:
            CircularCulvert(double Q, double D, double L, double So, double TW, double ke, int entrance_type)
                : Culvert(Q,D,L,So,TW,ke,entrance_type)
            {
            }

            double calculate_HWI()
            {
                double HW = dc + pow(calculate_v(), 2) / 2*g;
                double HW1;
                if(HW / D < 1.2)
                {
                    HW1 = pow(Q / 0.421 * pow(g, 0.5) * pow(D, 0.87) , 1.0 / 1.63);
                }else
                {
                    HW1 =  pow(Q / 0.530 * pow(g, 0.5) * pow(D, 1.87) , 1.0 / 0.63);
                }

                if(entrance_type == 2)
                {
                    return 0.92 * pow(HW1 / D, 0.9);
                }else if(entrance_type == 3)
                {
                    return 0.91 * pow(HW1 / D, 0.94);
                }

                return HW1;
            }

            void calculate_dc()
            {
                double dc_D = pow((Q * pow(D, 2.5)) / 1.26 * (pow(g, 0.5)), 1.0 / 3.75);
                if(dc_D > 0.85 && dc_D < 1)
                {
                    dc = dc_D * D;
                }else
                {
                    dc_D = pow((Q * pow(D, 2.5)) / 0.95 * (pow(g, 0.5)), 1.0 / 1.95);
                    dc = dc_D * D;
                }

                if(dc >= D)
                {
                    dc = D;
                }

            }

            double calculate_A()
            {
                return pow(D, 2) * PI / 4;
            }

            double calculate_Rh()
            {
                return calculate_fi() * D / 2;
            }

    };
}
