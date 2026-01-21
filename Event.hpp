#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <any>
#include <unordered_map>
class Event {
public:
    Event(int iLoop) : id_(iLoop) {}
    int loopID() const { return id_; }
    void setData(const std::string& key, const std::any& value) {
        data_[key] = value;
    }
    std::any getData(const std::string& key) const {
        auto it = data_.find(key);
        if (it != data_.end()) {
            return it->second;
        }
        return {};
    }
    void clear() {
        data_.clear();
    }
private:
    int id_;
    std::unordered_map<std::string, std::any> data_;
};
#endif
