#include <algorithm>
class Utils
{
public:
	static int f(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
    {
        int left = max(x1, x3);
        int top = min(y2, y4);
        int right = min(x2, x4);
        int bottom = max(y1, y3);

        int width = right - left;
        int height = top - bottom;

        if (width < 0 || height < 0)
            return 0;

        return width * height;
    }
};