#include <stdexcept>
#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <utility>
#include <algorithm>
#include <vector>

using namespace std;

static const size_t MAX_PAGES = 1000;

class Book {
    public:
    Book (istream& input=cin, ostream& out=cout): input_(input), out_(out), behind_(MAX_PAGES+1) {}

    void ReadRequest() {
        string req;
        if (input_ >> req) {
            if (req == "READ"s) {
                int user, page;
                if (input_ >> user >> page) {
                    Read(user, page);
                    return;
                }
            }
            else if (req == "CHEER"s) {
                int user;
                if (input_ >> user) {
                    out_ << setprecision(6) << Cheer(user) << endl;
                    return;
                }
            }
        }
        throw invalid_argument("Bad request"s);
    }

    private:
    istream& input_;
    ostream& out_;
    vector<int> behind_;
    unordered_map<int, int> users_;

    void Read(int user, int page) {
        auto it = behind_.begin();
        if (users_.count(user)) {
            it = behind_.begin() + users_.at(user) + 1;
            for_each(it, behind_.begin() + page + 1, [](auto& u_behind) {--u_behind;});
        }
        else {
            for_each(behind_.begin() + page + 1, behind_.end(), [](auto& u_behind) {++u_behind;});
        }
        users_[user] = page;
    }

    double Cheer(int user) const {
        if (!users_.count(user)) {
            return 0;
        }
        if (users_.size() == 1) {
            return 1;
        }
        return behind_.at(users_.at(user)) * 1.0 / (users_.size() - 1);
    }
};

int main() {
    Book book;
    int req_num;
    if (cin >> req_num) {
        for (int i = 0; i < req_num; ++i) {
            book.ReadRequest();
        }
    }
    return 0;
}
