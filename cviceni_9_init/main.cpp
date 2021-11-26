#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

using std::size_t;

class Writable {
public:
    virtual ~Writable() = default;
    virtual void write(std::ostream& out) const = 0;
};

class String : public Writable {
public:
    String(const std::string& str): m_str(str) {}
    virtual ~String() override = default;
    virtual void write(std::ostream& out) const override { out << m_str; }
private:
    std::string m_str;
};

class Number : public Writable {
public:
    Number(double num): m_num(num) {}
    virtual ~Number() override = default;
    virtual void write(std::ostream& out) const override { out << m_num; }
private:
    double m_num;
};

class Group : public Writable {
public:
    Group(const std::string& prefix, const std::string& separator, const std::string& suffix):
        m_prefix(prefix),
        m_separator(separator),
        m_suffix(suffix) {}

    virtual ~Group() override = default;

    void push_back(std::unique_ptr<Writable> child) {
        m_children.push_back(std::move(child));
    }

    virtual void write(std::ostream& out) const override {
        out << m_prefix;
        bool first = true;
        for (auto& child : m_children) {
            if (!first) {
                out << m_separator;
            }
            first = false;
            child->write(out);
        }
        out << m_suffix;
    }
private:
    std::vector<std::unique_ptr<Writable>> m_children;
    std::string m_prefix;
    std::string m_separator;
    std::string m_suffix;
};

std::ostream& operator<<(std::ostream& out, const Writable& writable) {
    writable.write(out);
    return out;
}

int main() {
    Group g("[", " | ", "]");
    g.push_back(std::make_unique<Number>(123));
    g.push_back(std::make_unique<String>("abc"));
    g.push_back(std::make_unique<Group>("(", ", ", ")"));

    std::cout << g << '\n';
}
