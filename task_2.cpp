#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <set>
#include <cassert>
#include <fstream>

using namespace std;

class Domain {
    public:
    Domain(string domain) {

        domain = {domain.rbegin(), domain.rend()};
        domain.push_back('.');
        domain_ = move(domain);
    }

    string GetDomain() const {
        return {domain_.rbegin(), domain_.rend()};
    }

    bool operator==(const Domain& other) const {
        return domain_ == other.domain_;
    }

    bool IsSubDomain(const Domain& other) const {
        if (other.domain_.size() < domain_.size()) {
            return false;
        }
        if (equal(other.domain_.begin(), other.domain_.begin() + domain_.size(), domain_.begin(), domain_.end())) {
            if (other.domain_.size() == domain_.size() || other.domain_.at(domain_.size() - 1) == '.') {
                return true;
            }
        }
        return false;
    }

    bool operator<(const Domain& other) const {
        return lexicographical_compare(domain_.begin(), domain_.end(), other.domain_.begin(), other.domain_.end(), [](char c1, char c2){
            return (c2 != '.') && (c1 < c2 || c1 == '.');
        });
    }

    private:
    string domain_;
};

Domain ReverseDomain(const Domain& d, bool add_dot = true) {
    string rev_s = d.GetDomain();
    reverse(rev_s.begin(), rev_s.end());
    if (add_dot) {
        rev_s.push_back('.');
    }
    return rev_s;
}

struct ReversedDomainComparator {
    bool operator()(const Domain& d1, const Domain& d2) const {
        return ReverseDomain(d1) < ReverseDomain(d2);
    }
};



class DomainChecker {
    public:
    template <typename Iterator>
    DomainChecker(Iterator range_begin, Iterator range_end){
        stop_domains = {range_begin, range_end};
        if (stop_domains.empty()) {
            return;
        }
        sort(stop_domains.begin(), stop_domains.end());

        auto new_end = unique(stop_domains.begin(), stop_domains.end(), [](const Domain& d1, const Domain& d2){
            return d1.IsSubDomain(d2);});

        stop_domains.erase(new_end, stop_domains.end());
    }

    bool IsForbidden(const Domain& domain) const {
        auto found = upper_bound(stop_domains.begin(), stop_domains.end(), domain);
        return found != stop_domains.begin() && (--found)->IsSubDomain(domain);
    }

    private:
    vector<Domain> stop_domains;
};

vector<Domain> ReadDomains(size_t num, istream& input = cin) {
    vector<Domain> domains;
    for (size_t i = 0; i < num; ++i) {
        string domain_name;
        getline(input, domain_name);
        domains.push_back({domain_name});
    }
    return domains;
}

template <typename Number>
    Number ReadNumberOnLine(istream& input = cin) {
        string line;
        getline(input, line);

        Number num;
        std::istringstream(line) >> num;

        return num;
    }


    int main() {
        const std::vector<Domain> forbidden_domains = ReadDomains(ReadNumberOnLine<size_t>());
        DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
        const std::vector<Domain> test_domains = ReadDomains(ReadNumberOnLine<size_t>());
        for (const Domain& domain : test_domains) {
            cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
        }
    }
