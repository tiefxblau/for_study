#include <iostream>
#include <vector>
using namespace std;

int getLowerMedian(const vector<int>& nums1, int l1, int r1, const vector<int>& nums2, int l2, int r2)
{
    if (l1 == r1)
        return max(nums1[l1], nums2[l2]);

    int m1 = (l1 + r1 + 1) >> 1;
    int m2 = (l2 + r2 + 1) >> 1;
    if (nums1[m1] == nums2[m2])
    {
        return nums1[m1];
    }
    else if (nums1[m1] > nums2[m2])
    {
        if ((r1 - l1 + 1) & 1)
            return getLowerMedian(nums1, l1, m1, nums2, m2, r2);
        else
            return getLowerMedian(nums1, l1, m1 - 1, nums2, m2, r2);
    }
    else
    {
        if ((r1 - l1 + 1) & 1)
            return getLowerMedian(nums1, m1, r1, nums2, l2, m2);
        else
            return getLowerMedian(nums1, m1, r1, nums2, l2, m2 - 1);
    }
}

int getUpperMedian(const vector<int>& nums1, int l1, int r1, const vector<int>& nums2, int l2, int r2)
{
    if (l1 == r1)
        return min(nums1[l1], nums2[l2]);

    int m1 = l1 + ((r1 - l1) >> 1);
    int m2 = l2 + ((r2 - l2) >> 1);
    if (nums1[m1] == nums2[m2])
    {
        return nums1[m1];
    }
    else if (nums1[m1] > nums2[m2])
    {
        if ((r1 - l1 + 1) & 1)
            return getUpperMedian(nums1, l1, m1, nums2, m2, r2);
        else
            return getUpperMedian(nums1, l1, m1, nums2, m2 + 1, r2);
    }
    else
    {
        if ((r1 - l1 + 1) & 1)
            return getUpperMedian(nums1, m1, r1, nums2, l2, m2);
        else
            return getUpperMedian(nums1, m1 + 1, r1, nums2, l2, m2);
    }
}

int getKthNumSortedArrays(const vector<int>& nums1, const vector<int>& nums2, int k)
{
    const vector<int>* longv = &nums1;
    const vector<int>* shortv = &nums2;
    if (nums2.size() > nums1.size())
        swap(longv, shortv);

    int longLen = longv->size();
    int shortLen = shortv->size();

    if (k <= shortLen)
    {
        return getUpperMedian(nums1, 0, k - 1, nums2, 0, k - 1);
    }
    else if (k > longLen)
    {
        return getLowerMedian(*shortv, k - longLen - 1, shortLen - 1
            , *longv, k - shortLen - 1, longLen - 1);
    }
    else
    {
        if ((*longv)[k - shortLen - 1] >= (*shortv)[shortLen - 1])
            return (*longv)[k - shortLen - 1];
        else
            return getUpperMedian(*shortv, 0, shortLen - 1
                , *longv, k - shortLen, k - 1);
    }

}

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    int len = nums1.size() + nums2.size();
    double res = 0.0;
    if (len & 1)
        res = getKthNumSortedArrays(nums1, nums2, len / 2 + 1);
    else
    {
        res = getKthNumSortedArrays(nums1, nums2, len / 2) +
            getKthNumSortedArrays(nums1, nums2, len / 2 + 1);
        res /= 2.0;
    }

    return res;
}

int main()
{
    vector<int> nums1 = { 1, 3, 4, 5, 6 };
    vector<int> nums2 = { 2 };
    cout << getLowerMedian(nums1, 0, nums1.size() - 1, nums2, 0, nums2.size() - 1) << endl;
    cout << getUpperMedian(nums1, 0, nums1.size() - 1, nums2, 0, nums2.size() - 1) << endl;
    cout << findMedianSortedArrays(nums1, nums2) << endl;

    return 0;
}