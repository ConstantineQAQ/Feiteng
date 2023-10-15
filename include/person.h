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
    Person();
    std::string getName() const { return m_name; }
    std::string getId() const { return m_id; }
    std::string getDepartment() const { return m_department; }
    FaceInfo::ptr getFaceInfo() const { return m_face; }
    Temperature::ptr getTemperature() const { return m_temperature; }
    std::string getLastSignInDate() { return m_lastSignInDate; }
    std::string getPhotoPath() { return m_facepath; }
    void setTemperature(Temperature::ptr temper) { m_temperature = temper; }
    void setName(const std::string& name) { m_name = name; }
    void setId(const std::string& id) { m_id = id; }
    void setFaceInfo(FaceInfo::ptr face) { m_face = face; }
    void setLastSignInDate(const std::string& date) { m_lastSignInDate = date; }
    void setDepartment(const std::string& department) { m_department = department; }
    void setPhotoPath(const std::string& path) { m_facepath = path; }
private:
    FaceInfo::ptr m_face; // 这个人的人脸信息
    Temperature::ptr m_temperature; // 这个人的体温信息
    std::string m_name; // 这个人的姓名
    std::string m_facepath; //  这个人的人脸图片路径
    std::string m_id; // 这个人的工号
    std::string m_department; // 这个人的部门
    std::string m_lastSignInDate; // 上次签到日期
};
} // namespace Feiteng


#endif // PEOPLE_H