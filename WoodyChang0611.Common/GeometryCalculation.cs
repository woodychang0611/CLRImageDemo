using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Shapes;


namespace WoodyChang0611
{
    
    public sealed partial class MathFunctions
    {
        static public Boolean IsPointNearby(Point point1, Point point2, Double distance)
        {
            if (Math.Abs(point1.X - point2.X) < distance && Math.Abs(point1.Y - point2.Y) < distance)
                return true;
            else
                return false;
        }
        //檢查兩線是否有交叉
        static public Boolean IsIntersected(Line line1, Line line2)
        {

            Double x1 = line1.X1;
            Double y1 = line1.Y1;
            Double x2 = line1.X2;
            Double y2 = line1.Y2;
            Double x3 = line2.X1;
            Double y3 = line2.Y1;
            Double x4 = line2.X2;
            Double y4 = line2.Y2;
            if (
                (x3 * (-y1 + y2) + x2 * (y1 - y3) + x1 * (-y2 + y3)) * (x4 * (-y1 + y2) + x2 * (y1 - y4) + x1 * (-y2 + y4)) <= 0 &&
                (x1 * (-y3 + y4) + x4 * (y3 - y1) + x3 * (-y4 + y1)) * (x2 * (-y3 + y4) + x4 * (y3 - y2) + x3 * (-y4 + y2)) <= 0
                ) return true;
            return false;

        }

        //檢查測試線是否與任何一條線交叉
        static public Boolean IsIntersected(Line testLine, IEnumerable<Line> lines)
        {
            foreach (Line line in lines)
            {
                if (IsIntersected(line, testLine))
                {
                    return true;
                }
            }
            return false;
        }
        static public Polyline GetSectionLine(Point startPoint, Point endPoint, Double width, Double height)
        {
            Polyline line = new Polyline();
            Double x1 = startPoint.X;
            Double y1 = startPoint.Y;
            Double x2 = endPoint.X;
            Double y2 = endPoint.Y;
            //垂直狀況
            if (x1 == x2)
            {
                line.Points.Add(new Point(x1, 0));
                line.Points.Add(new Point(x2, height));
            }
            //水平狀況
            else if (y1 == y2)
            {
                line.Points.Add(new Point(0, y1));
                line.Points.Add(new Point(width, y2));
            }
            else
            {
                Double slope = (y1 - y2) / (x1 - x2);
                //和y=0之交點
                Double xAt0 = x1 + (0 - y1) / slope;
                //和y=height之交點
                Double xAtHeight = x1 + (height - y1) / slope;
                //和y=0之交點
                Double yAt0 = y1 + (0 - x1) * slope;
                //和y=0之交點
                Double yAtWidth = y1 + (width - x1) * slope;
                //重新加入範圍內的點 正常只有兩點
                if (xAt0 >= 0 && xAt0 <= width)
                    line.Points.Add(new Point(xAt0, 0));
                if (xAtHeight >= 0 && xAtHeight <= width)
                    line.Points.Add(new Point(xAtHeight, height));
                if (yAt0 >= 0 && yAt0 <= height)
                    line.Points.Add(new Point(0, yAt0));
                if (yAtWidth >= 0 && yAtWidth <= height)
                    line.Points.Add(new Point(width, yAtWidth));

                //移除多的點 正常不會發生
                while (line.Points.Count > 2)
                {
                    line.Points.Remove(line.Points.Last());
                }
            }
            return line;
        }
    }

}
