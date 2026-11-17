/******************************
* Работа с файлами            *
* Вариант 10                  *
* Карпов Кирилл               *
******************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct Car {
  string brand;
  int year;
  string owner;
  string plateNumber;
};

vector<string> splitFields(const string& line) {
  vector<string> fields;
  size_t start = 0;

  while (true) {
    size_t separator = line.find(';', start);

    if (separator == string::npos) {
      fields.push_back(line.substr(start));
      break;
    }

    fields.push_back(line.substr(start, separator - start));

    start = separator + 1;
  }

  return fields;
}

bool readInt(const string& text, int& value) {
  istringstream parser(text);
  char extra;

  if (!(parser >> value)) {
    return false;
  }

  return !(parser >> extra);
}

bool readCars(const string& fileName, vector<Car>& cars) {
  string lineFile;
  vector<string> fields;
  Car tempCar;
  int countLineFile = 0;

  ifstream file(fileName);

  if (!file.is_open()) {
    cout << "INVALID: file not found\n";
    return false;
  }

  while (getline(file, lineFile)) {
    ++countLineFile;

    if (!lineFile.empty() && lineFile.back() == '\r') {
      lineFile.pop_back();
    }

    fields = splitFields(lineFile);

    if (fields.size() != 4 ||
      fields[0].empty() ||
      fields[2].empty() ||
      fields[3].empty() ||
      !readInt(fields[1], tempCar.year) ||
      tempCar.year < 1886 ||
      tempCar.year > 2026 ||
      cars.size() >= 100) {
      cout << "ERROR line " << countLineFile << '\n';
      return false;
    }

    tempCar.brand = fields[0];
    tempCar.owner = fields[2];
    tempCar.plateNumber = fields[3];

    cars.push_back(tempCar);
  }

  if (file.bad()) {
    cout << "INVALID: file read error\n";
    return false;
  }

  return true;
}

string createReport(const vector<Car>& cars, const string& brandCar) {
  ostringstream report;
  int countLineFile = 0;
  int sumAgeCar = 0;
  int oldYearCar = 0;
  int oldCarIndex = -1;

  report << "All the cars in the garage\n";

  for (const auto& car : cars) {
    report << car.brand << '\t'
      << car.owner << '\t'
      << car.plateNumber << '\t'
      << car.year << '\n';

    if (oldCarIndex == -1 || car.year < oldYearCar) {
      oldYearCar = car.year;
      oldCarIndex = static_cast<int>(&car - &cars[0]);
    }
  }

  report << "\nThe oldest car:\n";

  if (oldCarIndex == -1) {
    report << "NONE\n";
  } else {
    const Car& oldCar = cars[oldCarIndex];

    report << oldCar.brand << '\t'
      << oldCar.owner << '\t'
      << oldCar.plateNumber << '\t'
      << oldCar.year << '\n';
  }

  report << "\nCars of brand " << brandCar << '\n';

  for (const auto& car : cars) {
    if (car.brand == brandCar) {
      report << car.brand << '\t'
        << car.owner << '\t'
        << car.plateNumber << '\t'
        << car.year << '\n';

      ++countLineFile;
      sumAgeCar += 2026 - car.year;
    }
  }

  if (countLineFile == 0) {
    report << "No cars of this brand\n";
  } else {
    report << "Average age: " << fixed << setprecision(2)
      << static_cast<double>(sumAgeCar) / countLineFile
      << " years\n";
  }

  return report.str();
}

bool writeReport(const string& fileName, const string& reportText) {
  ofstream file(fileName);

  if (!file.is_open()) {
    return false;
  }

  file << reportText;

  return static_cast<bool>(file);
}

int main() {
  vector<Car> cars;
  string brandCar;
  string reportText;

  if (!readCars("input.txt", cars)) {
    return 1;
  }

  cout << "Enter the car brand you are interested in\n";
  getline(cin, brandCar);

  if (brandCar.empty()) {
    cout << "INVALID: empty brand\n";
    return 1;
  }

  reportText = createReport(cars, brandCar);

  cout << '\n' << reportText;

  if (!writeReport("output.txt", reportText)) {
    cout << "INVALID: output file error\n";
    return 1;
  }

  return 0;
}