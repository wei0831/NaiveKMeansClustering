/******************************************************************************/
/*!
\file   main.cpp
\author Jack Chang
\date   10/24/2015
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief
  Naive Implementation of K-Means Clustering
*/
/******************************************************************************/
//#include <vld.h>        // Visual Leak Detect for VS2013
#include <iostream>     // cout
#include <fstream>      // ifstream
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <cmath>

// 0: Don't Output Step Result; 1: Output Step Result;
#define STEP_DEBUG 1
// K Means Clustering
#define K_MEAN 3

// Type Definition
typedef std::vector<std::string> attrtable;
typedef std::unordered_multimap<std::string, std::vector<int>> datatable;
typedef std::vector<std::vector<int>> rawdatavector;

// Prototype Definition
void readData(const char * filename, attrtable& at, datatable& dt);
void preprocessData(attrtable& at, datatable& dt);
void outputResult(std::string filename, attrtable& at, rawdatavector c, datatable& dt);
void kmeanclustering(attrtable& at, datatable& dt, rawdatavector& c, unsigned int k);
double distance(std::vector<int>& org, std::vector<int>& cur, int first, int last);

int main(void)
{
  std::cout << "================= Read Data =================" << std::endl;
  attrtable at;
  datatable dt;
  rawdatavector centroids;
  readData("assets/TshirtData.csv", at, dt);
  preprocessData(at, dt);
  std::cout << "================ Done Read Data ===============" << std::endl;

  std::cout << "================= Process Data =================" << std::endl;
  kmeanclustering(at, dt, centroids, K_MEAN);
  std::cout << "================ Done Process Data ===============" << std::endl;

  std::cout << "================= Output Data =================" << std::endl;
  outputResult("Result", at, centroids, dt);
  std::cout << "=============== Done Output Data ==============" << std::endl;

  return 0;
}

void kmeanclustering(attrtable& at, datatable& dt, rawdatavector& c, unsigned int k)
{
  assert(dt.size() >= k);

  int step = 0;
  auto it = dt.begin();
  unsigned idx_group = it->second.size() - 1;   // Index of group#

  // Pick k number of sample centroids
  // Or could be random
  for (unsigned i = 0; i < k; ++i)
  {
    it->second[idx_group] = i;
    c.push_back(it->second);
    ++it;
  }

  // While the centroids are still moving around
  while(true)
  {
    // Assign each row of data the group# which is same as the closest centroid
    for (it = dt.begin(); it != dt.end(); ++it)
    {
      int group = 0;
      double smallest = distance(c[0], it->second, 0, idx_group - 1); // ignore the last attribute: group#

      for (unsigned i = 1; i < k; ++i)
      {
        double d = distance(c[i], it->second, 0, idx_group - 1);
        if (d < smallest)
        {
          smallest = d;
          group = c[i][idx_group];
        }
      }
      it->second[idx_group] = group;
    }
    // Debug Output
    if(STEP_DEBUG)
      outputResult("step" + std::to_string(step), at, c, dt);

    // Figure out new centroids
    rawdatavector newCentroid;
    std::vector<int> counts(k, 0);
    for (unsigned i = 0; i < k; ++i)
    {
      newCentroid.push_back(std::vector<int>(idx_group + 1, 0));
      newCentroid[i][idx_group] = i;
    }

    // Sum up the data in each attribute in the same group
    for (auto& data : dt)
    {
      int group = data.second[idx_group];
      for (unsigned i = 0; i < idx_group; ++i)
      {
        newCentroid[group][i] += data.second[i];
      }
      counts[group] += 1;
    }

    // Average the sum for each attribute in each group to get the new centroid
    bool modified = false;  // To check if the centroids have moved or not
    for (unsigned i = 0; i < k; ++i)
    {
      for (unsigned j = 0; j < idx_group; ++j)
      {
        int average = (int)(newCentroid[i][j] / counts[i]);
        if (c[i][j] != average)
        {
          modified = true;
          c[i][j] = average;
        }
      }
    }
    // If centroids stay the same, we can now exit.
    if (!modified)
      return;

    ++step;
  }
}

void outputResult(std::string filename, attrtable& at, rawdatavector c, datatable& dt)
{
  // Output Centroid Data
  std::ofstream centroid(filename + "_c.txt", std::ofstream::out);
  // Write Attributes
  for (auto atr = at.begin() + 1; atr != at.end(); ++atr)
  {
    centroid << *atr;
    if((atr + 1)!= at.end())
      centroid << ",";
  }
  centroid << std::endl;
  // Write Data
  for (auto& tuple : c)
  {
    for (auto data = tuple.begin(); data != tuple.end(); ++data)
    {
      centroid << *data;
      if ((data + 1) != tuple.end())
        centroid << ",";
    }
    centroid << std::endl;
  }
  centroid.close();

  // Output Dataset
  std::ofstream dataset(filename + "_d.txt", std::ofstream::out);
  // Write Attributes
  for (auto atr = at.begin(); atr != at.end(); ++atr)
  {
    dataset << *atr;
    if ((atr + 1) != at.end())
      dataset << ",";
  }
  dataset << std::endl;
  // Write Data
  for (auto& tuple : dt)
  {
    dataset << tuple.first << ",";
    for (auto data = tuple.second.begin(); data != tuple.second.end(); ++data)
    {
      dataset << *data;
      if ((data + 1) != tuple.second.end())
        dataset << ",";
    }
    dataset << std::endl;
  }
  dataset.close();
}

void readData(const char * filename, attrtable& at, datatable& dt)
{
  // Open file
  std::ifstream infile(filename, std::ifstream::in);
  std::string line;
  if (infile.fail()) {
    std::cout << "Failed to read file." << std::endl;
    return;
  }    

  // Read Attributes
  std::getline(infile, line);
  auto start = line.begin();
  auto end = line.begin();
  while (end != line.end())
  {
    if (*end == ',')
    {
      at.push_back(std::string(start, end));
      start = ++end;
    }
    else
    {
      ++end;
    }
  }
  if (start != end)
  {
    at.push_back(std::string(start, end));
  }

  // Read Data
  while (std::getline(infile, line))
  {
    start = line.begin();
    end = line.begin();

    while (end != line.end() && *end != ',')
    {
      ++end;
    }
    assert(end < line.end());
    auto instance = dt.emplace(std::string(start, end), std::vector<int>());
    start = ++end;

    while (end != line.end())
    {
      if (*end == ',')
      {
        instance->second.push_back(std::stoi(std::string(start, end), nullptr));
        start = ++end;
      }
      else
      {
        ++end;
      }
    }
    if (start != end)
    {
      instance->second.push_back(std::stoi(std::string(start, end), nullptr));
    }
  }
}

void preprocessData(attrtable& at, datatable& dt)
{             
  // Add Group Classification
  at.push_back("group");
  // Initial group as -1
  for (auto &d : dt)
  {
    d.second.push_back(-1);
  }
}

// Calculate euclidean distance of two sets of data
double distance(std::vector<int>& org, std::vector<int>& cur, int first, int last)
{
  int sum = 0;
  for (int i = first; i <= last; ++i)
  {
    int diff = org[i] - cur[i];
    sum += diff * diff;
  }
  return sqrt(sum);
}
