#include <iostream>
#include <string>
#include <memory>
#include <vector>

class Writable {
public:
    virtual ~Writable() = default;
    virtual void write(std::ostream& out) const = 0;
};

class String : public Writable {
public:
    String(const std::string& str) : m_str(str) {}
    virtual ~String() override = default;
    virtual void write(std::ostream& out) const override { out << m_str << '\n'; }
private:
    std::string m_str;
};

class Number : public Writable {
public:
    Number(double num) : m_num(num) {}
    virtual ~Number() override = default;
    virtual void write(std::ostream& out) const override { out << m_num << '\n'; }
private:
    double m_num;
};

/*int main() {
    std::unique_ptr<Writable> str1(new String("abc"));
    std::unique_ptr<Writable> str2(new String("def"));
    std::unique_ptr<Writable> num1(new Number(123));
    std::unique_ptr<Writable> num2(new Number(456));

    str1->write(std::cout);
    str2->write(std::cout);
    num1->write(std::cout);
    num2->write(std::cout);
}*/

class Group : public Writable{
public:
    Group(const std::string& start, const std::string& end, const std::string& delimit) : startCharacter(start), terminateCharacter(end), delimiter(delimit) {}
    virtual ~Group() override = default;
    virtual void write(std::ostream& out) const override{
        out << startCharacter;
        size_t i = 0;
        for(auto& groupMember : group){
            if(i!=0){
                out << delimiter;
            } else {
                i++;
            }
            groupMember->write(out);
        }
        out << delimiter;
    }
    void push_back(Writable* blob){
        group.push_back(std::unique_ptr<Writable>(blob));
    }
private:
    std::string startCharacter;
    std::string terminateCharacter;
    std::string delimiter;
    std::vector<std::unique_ptr<Writable>> group;
};

int main() {
    Group g("(", ", ", ")");
    g.push_back(new Number(123));
    g.push_back(new Number(456));
    g.push_back(new String("abc"));
    g.push_back(new String("def"));

    std::unique_ptr<Group> gp(new Group("[", " | ", "]"));
    gp->push_back(new Number(789));
    gp->push_back(new String("ghi"));
    g.push_back(gp.release());

    g.write(std::cout); // (123, 456, abc, def, [789 | ghi])
    std::cout << '\n';
}

