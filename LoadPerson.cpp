struct DB_Info {
    string name;
    int connection_timeout;
    bool allow_exceptions;
    DBLogLevel log_level;
};

struct PersonTemplate {
    int min_age;
    int max_age;
    string name_filter;
};


vector<Person> LoadPersons(const DB_Info& db_inf, const PersonTemplate& person_templ) {
    DBConnector connector(db_inf.allow_exceptions, db_inf.log_level);
    DBHandler db;
    if (db_inf.name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_inf.name, db_inf.connection_timeout);
    } else {
        db = connector.Connect(db_inf.name, db_inf.connection_timeout);
    }
    if (!db_inf.allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << person_templ.min_age << " and "s << person_templ.max_age << " "s
              << "and Name like '%"s << db.Quote(person_templ.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}
