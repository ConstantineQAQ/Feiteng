#ifndef PEOPLE_H
#define PEOPLE_H

#include <string>
#include <memory>

#include "face.h"
#include "log.h"
#include "temperature.h"

namespace Feiteng
{
class Person
{
public:
    typedef std::shared_ptr<Person> ptr;
    Person(std::string name, std::string id, std::string department);
    std::string getName() const { return m_name; }
    std::string getId() const { return m_id; }
    std::string getDepartment() const { return m_department; }
    FaceInfo::ptr getFaceInfo() const { return m_face; }
    Temperature::ptr getTemperature() const { return m_temperature; }
    void setFaceInfo(FaceInfo::ptr face) { m_face = face; }
    void setLastSignInDate(const std::string& date) { m_lastSignInDate = date; }
    std::string getLastSignInDate() const { return m_lastSignInDate; }
private:
    FaceInfo::ptr m_face; // 这个人的人脸信息
    Temperature::ptr m_temperature; // 这个人的体温信息
    std::string m_name; // 这个人的姓名
    std::string m_id; // 这个人的工号
    std::string m_department; // 这个人的部门
    std::string m_lastSignInDate; // 上次签到日期
};
} // namespace Feiteng


#endif // PEOPLE_H