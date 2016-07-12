#ifndef JSON_IS_AMALGAMATION 
#define JSON_IS_AMALGAMATION
#endif // !JSON_IS_AMALGAMATION 

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include "./json/json.h"
#include "./json/json-forwards.h"
using namespace std;
struct products
{
	string name;
	string products_man;
	string family;
	string model;

	vector<int> vec;
}products1[705];
struct listings
{
	string title;
	string listings_man;
	string currency;
	string price;
};
vector<listings> listings1;
ifstream in_products;
ifstream in_listings;
ofstream out_result("results.txt");
Json::Value listing;
Json::Value product;
Json::Reader reader;
Json::FastWriter writer;
void init();
int judge(int i, int j);
int judge2(string string1, string string2, size_t pos);
void out();
int main()
{
	init();
	int i, j;
	for (i = 0; i < 20000; i++)
	{
		int weight;
		vector<pair<int, int>> judgeres;
		for (j = 0; j < 700; j++)
		{
			weight = judge(i, j);
			if (weight == 0) continue;
			judgeres.push_back(make_pair(j, weight));
		}
		if (judgeres.size() == 0) { continue; }
		sort(judgeres.begin(), judgeres.end(), [](pair<int, int> p1, pair<int, int> p2) {return p1.second > p2.second; });
		int maxweight = judgeres[0].second;
		for (j = 0; j < judgeres.size(); j++)
		{
			if (judgeres[j].second < maxweight) break;
			products1[judgeres[j].first].vec.push_back(i);
		}
	}
	out();
	return 0;
}
void init()
{
	in_products.open("products.txt");
	in_listings.open("listings.txt");
	char pro_buffer[500] = { 0 };
	char list_buffer[1000] = { 0 };
	int pro_num, list_num;
	for (pro_num = 0; pro_num < 700; pro_num++)
	{
		in_products.getline(pro_buffer, 500);
		reader.parse(pro_buffer, product);
		products1[pro_num].name = product["product_name"].asString();
		products1[pro_num].products_man = product["manufacturer"].asString();
		string familytemp = product["family"].asString();
		products1[pro_num].family = familytemp == "" ? "???" : familytemp;
		products1[pro_num].model = product["model"].asString();
	}
	for (list_num = 0; list_num < 20000; list_num++)
	{
		listings templisting;
		in_listings.getline(list_buffer, 1000);
		reader.parse(list_buffer, listing);
		templisting.title = listing["title"].asString();
		templisting.currency = listing["currency"].asString();
		templisting.price = listing["price"].asString();
		templisting.listings_man = listing["manufacturer"].asString();
		listings1.push_back(templisting);
	}
	return;
}
int judge(int i, int j)
{
	int weight = 0;
	string listing_title = listings1[i].title;
	string listing_man = listings1[i].listings_man;
	string product_man = products1[j].products_man;
	string product_family = products1[j].family;
	string product_model = products1[j].model;
	size_t pos11, pos12, pos2, pos3;
	pos11 = listing_title.find(product_man);
	pos12 = listing_man.find(product_man);
	pos2 = listing_title.find(product_family);
	pos3 = listing_title.find(product_model);
	bool temp = false;
	if (!judge2(listing_title, product_man, pos11) && !judge2(listing_man, product_man, pos12)) return 0;
	else weight += (int)product_man.length();
	if (!judge2(listing_title, product_family, pos2)) return 0;
	else weight += (int)product_family.length();
	if (!judge2(listing_title, product_model, pos3)) return 0;
	else weight += (int)product_model.length();
	return weight;
}
int judge2(string string1, string string2, size_t pos)
{
	if (pos == string::npos) return 0;
	int next = (int)pos + (int)string2.length();
	if (pos == 0)
	{
		if (!iswalpha(string1[next]) && !iswdigit(string1[next])) return 1;
		else return 0;
	}
	else if (next == string1.length())
	{
		if (!iswalpha(string1[pos - 1]) && !iswdigit(string1[pos - 1])) return 1;
		else return 0;
	}
	else
	{
		if (!iswalpha(string1[next]) && !iswdigit(string1[next]) && !iswalpha(string1[pos - 1]) && !iswdigit(string1[pos - 1])) return 1;
		else return 0;
	}
}
void out()
{
	int i, j;
	for (i = 0; i < 700; i++)
	{
		Json::Value output;
		Json::Value templist;
		output["product_name"] = products1[i].name;
		for (j = 0; j < products1[i].vec.size(); j++)
		{
			templist["title"] = listings1[products1[i].vec[j]].title;
			templist["manufacturer"] = listings1[products1[i].vec[j]].listings_man;
			templist["currency"] = listings1[products1[i].vec[j]].currency;
			templist["price"] = listings1[products1[i].vec[j]].price;
			output["listings"].append(templist);
		}
		out_result << writer.write(output);
	}
}
