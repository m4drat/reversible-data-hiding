
#include "image/bmp_image.h"

class References {
public:
    References()
    {
        m_Airplane = rdh::BmpImage("..\\..\\..\\..\\images\\original\\airplane.bmp");
        m_Boat = rdh::BmpImage("..\\..\\..\\..\\images\\original\\boat.bmp");
        m_Crowd = rdh::BmpImage("..\\..\\..\\..\\images\\original\\crowd.bmp");
        m_LenaGray = rdh::BmpImage("..\\..\\..\\..\\images\\original\\lena_gray.bmp");
        m_Man = rdh::BmpImage("..\\..\\..\\..\\images\\original\\man.bmp");
    }

    static References& GetInstance() {
        static References INSTANCE;
        return INSTANCE;
    }

    rdh::BmpImage GetAirplane() const { return m_Airplane; }
    rdh::BmpImage GetBoat() const { return m_Boat; }
    rdh::BmpImage GetCrowd() const { return m_Crowd; }
    rdh::BmpImage GetLena() const { return m_LenaGray; }
    rdh::BmpImage GetMan() const { return m_Man; }

private:
    rdh::BmpImage m_Airplane{ 0, 0 };
    rdh::BmpImage m_Boat{ 0, 0 };
    rdh::BmpImage m_Crowd{ 0, 0 };
    rdh::BmpImage m_LenaGray{ 0, 0 };
    rdh::BmpImage m_Man{ 0, 0 };
};