#include<iostream>
#include<vector>
#include<cmath>
#include<tuple>
#include<algorithm>
using namespace std;/*this is kdtree for 2d with node as corrdinates and then internal
						structure is of binary seach tree*/
class Ktree{ 
public:
	tuple<int,int> node;//skeleton of kdtree
	Ktree* L=nullptr;
	Ktree* R=nullptr;

	Ktree(tuple<int,int> node)
	{
		this->node=node;
	}
};

class fun{
	vector<tuple<int,int>> corr;//

public:
	fun(vector<tuple<int,int>> p)
	{
		for(auto& i : p)
		{
			corr.push_back(i);
		}
	}
	void print(Ktree* tree)
	{
		if(!tree) return;
		cout<<"[";
		cout<<"("<<get<0>(tree->node)<<","<<get<1>(tree->node)<<")  ";

		if(tree->L || tree->R){
			cout<<"[";
			print(tree->L);
			cout<<"|";
			print(tree->R);
			cout<<"]";
		}
	}
	
	Ktree* kdtree(vector<tuple<int,int>>& corr,int d=0)
	{
		if(corr.empty()) return nullptr;

		int axis=d%2;
		if(axis==0){
				sort(corr.begin(),corr.end(),[](tuple<int,int>& a,tuple<int,int>& b){
						return get<0>(a) < get<0>(b);
				});}
		else{
			sort(corr.begin(), corr.end(), [](tuple<int, int>& a, tuple<int, int>& b) {
                return get<1>(a) < get<1>(b);
            });
		}

		int midIDX=corr.size()/2;
		tuple<int,int> mid=corr[midIDX];

		Ktree* node =new Ktree(mid);

		vector<tuple<int,int>> left(corr.begin(),corr.begin()+midIDX);
		vector<tuple<int,int>> right(corr.begin()+midIDX+1,corr.end());

		node->L=kdtree(left,d+1);
		node->R=kdtree(right,d+1);

		return node;
	}
	tuple<int,int> query_point(tuple<int,int> q,Ktree* tree,int d=0)
	{
		if(!tree) return {-1,-1};

		auto distence=[](tuple<int,int> a,tuple<int,int> b){
			return sqrt(pow(get<0>(a)-get<0>(b),2)+pow(get<1>(a)-get<1>(b),2));
		};

		tuple<int,int> nearest=tree->node;
		double dist=distence(q,nearest);
		
		int axis=d%2;

		Ktree* nextBranch=nullptr;
		Ktree* oppositeBranch=nullptr;

		if((axis==0 && get<0>(q)<get<0>(tree->node)) || (axis==1 && get<1>(q)<get<1>(tree->node)))
		{
			nextBranch=tree->L;
			oppositeBranch=tree->R;
		}
		else
		{
			nextBranch=tree->R;
			oppositeBranch=tree->L;
		}


		tuple<int,int>nextNearest=query_point(q,nextBranch,d+1);
		double nextDist=distence(q,nextNearest);

		if(nextDist<dist)
		{
			dist=nextDist;
			nearest=nextNearest;
		}

		if((axis==0 && abs(get<0>(q)-get<0>(tree->node))<dist) || 
			(axis==1 && abs(get<1>(q)-get<1>(tree->node))<dist))
		{
			tuple<int,int> oppositeBest=query_point(q,oppositeBranch,d+1);
			double oppositeDist=distence(q,oppositeBest);

		if(oppositeDist<dist)
		{
			dist=oppositeDist;
			nearest=oppositeBest;
		}
		}

		return nearest;
	}




};
int main()
{
	vector<tuple<int,int>> i={
        {3, 6}, {17, 15}, {13, 15}, {6, 12}, {9, 1}, {2, 7}, {10, 19}};
    	fun inp(i);

    	Ktree* root = inp.kdtree(i);
    	// inp.print(root);
    	tuple<int,int> query={14,5};
    	auto ans=inp.query_point(query,root);
    	cout<<get<0>(ans)<<" "<<get<1>(ans);
    	
	
}