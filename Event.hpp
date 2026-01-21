#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <any>
#include <unordered_map>
class Event {
public:
friend class EventLoopController; // Allow EventLoopController to access private members
    Event(int iLoop) : loopId_(iLoop) {}
    int id() const { return id_; }
    int loopID() const { return loopId_; }
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
private:
    //called by framework
    void clear() {
        data_.clear();
    }
    //called by framework
    void setId(int id) {
        id_ = id;
    }

    int loopId_;
    int id_ = 0;
    std::unordered_map<std::string, std::any> data_;
};
#endif
