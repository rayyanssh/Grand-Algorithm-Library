


deque<pair<int, int>> q;
int cnt_added = 0;
int cnt_removed = 0;
int minimum = q.front().first;
while (!q.empty() && q.back().first > new_element)
    q.pop_back();
q.push_back({new_element, cnt_added});
cnt_added++;
if (!q.empty() && q.front().second == cnt_removed) 
    q.pop_front();
cnt_removed++;