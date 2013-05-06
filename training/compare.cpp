#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <utility>

// ===================================================================
// get the mean and sd for the new movie and best matched movie
// ===================================================================
std::vector<std::pair<float, float> > readMeans(int new_m, int match){
  std::ifstream inputFile("mean_ratings.txt");
  if(!inputFile){
    std::cerr << "ERROR: No mean ratings file" << std::endl;
    exit(0);
  }
  
  // fill 2 empty vectors
  std::vector<std::pair<float, float> > tmp(2, std::make_pair(0.0, 0.0));
  int count = 0;
  std::string line;
  
  // flag to assist classification
  // 1 if the new movie 2 if the best match
  int flag = 0;
  
  // temp vars
  float mean;
  float sd;
  
  // helps break early to reduce run time
  int i = 0;

  // get first number
  getline(inputFile, line, ',');
  // while not end of file
  while(!inputFile.eof()){
    // if it's the movie id check if matches
    if(count % 9 == 0){
      if(atoi(line.c_str()) == new_m)
	flag = 1;
      else if (atoi(line.c_str()) == match)
	flag = 2;
      getline(inputFile, line, ',');
    }
    // if it is the mean and of a row that matters grab it
    else if(count % 9 == 6){
      if(flag > 0)
	mean = atof(line.c_str());
      getline(inputFile, line, ',');
    }
    // same with standard deviation
    else if(count % 9 == 7){
      if(flag > 0)
	sd = atof(line.c_str());
      getline(inputFile, line);
    }
    // if last column of important row add to vector
    else if(count % 9 == 8){
      if(flag == 1){
	tmp[0] = std::make_pair(mean, sd);
	i++;
      }
      else if(flag == 2){
	tmp[1] = std::make_pair(mean, sd);
	i++;
      }
      flag = 0;
      getline(inputFile, line, ',');
    }
    else
      getline(inputFile, line, ',');

    if(i == 2) break;
    count++;
  }

  return tmp;
}

// ===================================================================
// compare the vectors of customer ids to see how many matches there are
// ===================================================================
int compare(const std::vector<int> &old_v, const std::vector<int> &new_v){
  int matches = 0;
  for(unsigned int i = 0; i < old_v.size(); i++){
    for(unsigned int j = 0; j < new_v.size(); j++){
      if(old_v[i] == new_v[j])
	matches++;
    }
  }
  return matches;
}


// ===================================================================
// Reads in the customer ids to vector 
// ===================================================================
int read_movie(std::vector<int> &movie,const std::string &filename,
		int customer_id){

  // check if you can read the file 
  std::ifstream inputFile(filename.c_str());
  if(!inputFile){
    std::cerr << "ERROR: cannot read input file " << filename.c_str() << std::endl;
    exit(0);
  }
  
  // set up the line variable each line will be put in
  std::string line;
  // gets the starting line
  getline(inputFile, line);
  
  // counter, flag and rating variables
  int count = 0;
  bool flag = 0;
  int customer_rating = 0;
  // while not at the end of the file iterate
  while(!inputFile.eof()){
    // if the count % 3 = 0 then it's the customer id
    if(count % 3 == 0){
      // add the customer id
      movie.push_back(atoi(line.c_str()));
      
      // if the customer id matches the id of our customer
      if(atoi(line.c_str()) == customer_id)
	flag = 1;
      
      getline(inputFile, line, ',');
    } 
    // if the count % 3 = 1 then it's the rating column
    else if(count % 3 == 1){
      // if this the line of the customer we want then grab the rating
      if(flag){
	customer_rating = atoi(line.c_str());
	flag = 0;
      }
      // get the next line by newline character not ','
      getline(inputFile, line);
    } 
    // else it is the date column which we won't use
    else{ getline(inputFile, line, ','); }
    // increment the counter
    count++;
  }

  return customer_rating;
}

// ===================================================================
// gets the filename from a movie id number
// ===================================================================
std::string getFilename(std::string movie_id){
  // used to get the appropriate name for the file that we want to open
  std::string zeroes(7 - int(movie_id.length()),'0');
  
  return "files/mv_" + zeroes + movie_id + ".txt";
}

// ===================================================================
// main function 
// ===================================================================
int main(int argc, char * argv[]){
  
  // read in movie in question via cmd line
  if(argc < 3){
    std::cerr << "usage: " << argv[0] << 
      " batch filename" << std::endl;
    std::cerr << "usage: " << argv[0] << 
      " single customer_id movie_id" << std::endl;
    return -1;
  }
  // vector of pairs of strings which are the test cases
  std::vector<std::pair<std::string,std::string> > cases;

  if(argc == 4)
    cases.push_back(std::make_pair(argv[2],argv[3]));
  else{
    std::ifstream inputFile(argv[2]);
    if(!inputFile){
      std::cerr << "ERROR: cannot read input file " 
		<< argv[2] << std::endl;
      return -1;
    }
    std::string s1;
    std::string s2;
    while(!inputFile.eof()){
      getline(inputFile, s1, '\t');
      getline(inputFile, s2);
      if(inputFile.eof()) break;
      cases.push_back(std::make_pair(s1,s2));
    }
  }

  std::vector<std::pair<float, float> > stats;
  float normal;
  float rating;
  int best_match_rating = 0;
  int movie_match = 0;
  for(unsigned int i = 0; i < cases.size(); i++){
    // get the filename from the movie id
    std::string new_file = getFilename(cases[i].second);
    
    // open that movie file and read in numbers to a vector
    std::vector<int> new_customer_ids;
    int customer_rating = read_movie(new_customer_ids, new_file, 
				     atoi(cases[i].first.c_str()));

    // iterate through ever movie and compare for matches
    // first check for it being a movie the customer in question watched or break
    std::vector<int> old_customer_ids;
    std::vector<int> best_match;
    int matches = 0;

    for(int j = 1; j <= 17770; j++){
      old_customer_ids.clear();
      // make the filename
      std::stringstream ss;
      ss << j;
      std::string s = ss.str();
      // make sure we aren't using the same movie we are predicting
      if ( s == cases[i].second)
	continue;
      // get the filename
      std::string filename = getFilename(s);
      
      // get the vector of id's and customer rating for the movie
      int customer_rating = read_movie(old_customer_ids, 
				       filename, atoi(cases[i].first.c_str()));
      // means the customer didn't watch the movie and we don't need to check it
      if (customer_rating == 0)
	continue;

      int tmp = compare(old_customer_ids, new_customer_ids);

      if(tmp > matches){
	matches = tmp;
	movie_match = j;
	best_match_rating = customer_rating;
	best_match = old_customer_ids;
      }
    }

    // find average and sd of both movies and rating customer gave
    stats = readMeans(atoi(cases[0].second.c_str()), movie_match);
    
    // standardize and translate over to new movie
    normal = (float(best_match_rating) - stats[1].first)/stats[1].second;
    rating = normal*stats[0].second + stats[0].first;

    std::cout << cases[i].second << ',' << stats[0].first << ','
	      << stats[0].second << ',' << rating << ','
	      << movie_match << ',' << stats[1].first << ','
	      << stats[1].second << ',' << best_match_rating << ','
	      << customer_rating << ',' << cases[i].first << std::endl;
  }

  if(argc == 4){
    // helpful print statments 
    std::cout.precision(3);
    std::cout << "\t\tMovies\tMean\tStd Dev\tRating\n" 
	      << "New:\t\t"<< argv[3] << '\t' 
	      << stats[0].first << '\t' << stats[0].second << '\t'
	      << rating
	      << "\nBest Match:\t" << movie_match 
	      << '\t' << stats[1].first << '\t' << stats[1].second << '\t'
	      << best_match_rating << std::endl;
  }

}
