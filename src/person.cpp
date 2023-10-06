#include "person.h"

namespace Feiteng
{
Feiteng::Person::Person(std::string name, std::string id, std::string department)
:m_name(name)
,m_id(id)
,m_department(department)
{
    m_face = FaceInfo::ptr(new FaceInfo(m_name));
    m_temperature = Temperature::create(TempMode::Body);
}

}
