#define BuildCountrySetter(FieldName, Type, private_var_)\
    BuildCountry& Set##FieldName(const Type& value) {\
        private_var_ = value;\
        return *this;\
    }

class BuildCountry {
public:
    BuildCountrySetter(Name, string, name_)
    BuildCountrySetter(IsoCode, string, iso_code_)
    BuildCountrySetter(PhoneCode, string, phone_code_)
    BuildCountrySetter(TimeZome, string, name_)
    BuildCountrySetter(Languages, vector<Language>, language_)

    BuildCountry& AddLanguage(const Language& language) {\
        languages_.push_back(language);
        return *this;\
    }

    operator Country() {
        return {name_, iso_code_, phone_code_, time_zone_, language_};
    }
private:
    string name_;
    string iso_code_;
    string phone_code_;
    string time_zone_;
    vector<Language> languages_;
};

template <typename CityObject>
City MakeCity(const Country& country, const CityObject& city_obj) {
    string city_name = city_obj["name"s].AsString(),
    city_iso_code = city_obj["iso_code"s].AsString(),
    phone_code = country.phone_code + city_obj["phone_code"s].AsString();

    return move({name,
            iso_code,
            phone_code,
            country.name,
            country.iso_code,
            country.time_zone,
            country.languages});
}


// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(vector<City>& cities, const Json& json, const Country& country) {
    for (const auto& city_json : json.AsList()) {
        cities.push_back(MakeCity(country, city_json.AsObject()));
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();

        BuildCountry country_builder = BuildCountry().
        SetName(country_obj["name"s].AsString()).
        SetIsoCode(country_obj["iso_code"s].AsString()).
        SetPhoneCode(country_obj["phone_code"s].AsString()).
        SetTimeZone(country_obj["time_zone"s].AsString());
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country_builder.AddLanguage(FromString<Language>(lang_obj.AsString()));
        }

        countries.push_back(country_builder);
        ParseCitySubjson(cities, country_obj["cities"s], countries.back());
    }
}
