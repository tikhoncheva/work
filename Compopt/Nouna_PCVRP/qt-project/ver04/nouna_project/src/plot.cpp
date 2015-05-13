#include <hrd/plot.h>


QVector<double> highlightx;
QVector<double> highlighty;

std::vector<unsigned int> getShortestWay(unsigned int source, unsigned int target,
                                         const std::vector<unsigned int>& _prev)
{
    std::vector<unsigned int> shortestWay;

    unsigned int u = target;

    shortestWay.push_back(u);

    while (_prev[u] != source)
    {
        u = _prev[u];
        shortestWay.push_back(u);
    }

    shortestWay.push_back(source);
    std::reverse(shortestWay.begin(), shortestWay.end());

    return shortestWay;
}

void plot_villages(QCustomPlot *plot,
                   std::vector<stVillage> V)
{
    //plot->clearItems();
    plot->clearGraphs();
    plot->replot();

    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(QBrush(Qt::white));
    myScatter.setSize(5);

    // create graph and assign data to it:
    plot->addGraph();
    assert (plot->graphCount()==1); // zu diesem Zeitpunkt soll nur ein Graph existieren
    plot->graph(0)->setScatterStyle(myScatter);
    plot->graph(0)->setLineStyle( QCPGraph::lsNone);
    // give the axes some labels:
    plot->xAxis->setLabel("longitude");
    plot->yAxis->setLabel("latitude");

    unsigned int nV= V.size();
    QVector<double> x(nV), y(nV); // initialize with entries 0..100
    QVector<double> xNouna(1), yNouna(1); // initialize with entries 0..100
    double xmin,xmax;
    double ymin, ymax;

    for (unsigned int i=0; i< nV; ++i)
    {
        x[i] = V[i].coord.first;
        y[i] = V[i].coord.second;
    }

    QVector<double>::iterator it_min =  std::min_element(x.begin(), x.end());
    xmin = *it_min;
    QVector<double>::iterator it_max =  std::max_element(x.begin(), x.end());
    xmax = *it_max;

    it_min =  std::min_element(y.begin(), y.end());
    ymin = *it_min;
    it_max =  std::max_element(y.begin(), y.end());
    ymax = *it_max;

    plot->graph(0)->setData(x, y);

    if (highlightx.size()>0)
    {
        plot->addGraph();
        myScatter.setBrush(QBrush(Qt::blue));
        myScatter.setSize(7);
        plot->graph(1)->setScatterStyle(myScatter);
        plot->graph(1)->setLineStyle( QCPGraph::lsNone);
        plot->graph(1)->setData(highlightx, highlighty);
    }


    // extra mark capital
    xNouna[0] = V[142-101].coord.first;
    yNouna[0] = V[142-101].coord.second;

    int ind = plot->graphCount();

    plot->addGraph();
    myScatter.setBrush(QBrush(Qt::red));
    myScatter.setSize(7);
    plot->graph(ind)->setScatterStyle(myScatter);
    plot->graph(ind)->setData(xNouna, yNouna);

    // set axes ranges, so we see all data:
    plot->xAxis->setRange(xmin-0.1, xmax+0.1);
    plot->yAxis->setRange(ymin-0.1, ymax+0.1);

    plot->replot();
}

void plot_labelsVillages(QCustomPlot *plot,
                         std::vector<stVillage> V)
{
    unsigned int nV= V.size();
    QVector<double> x(nV), y(nV); // initialize with entries 0..100

    for (unsigned int i=0; i< nV; ++i)
    {
        x[i] = V[i].coord.first;
        y[i] = V[i].coord.second;

        QCPItemText *textLabel = new QCPItemText(plot);
        textLabel->setText(QString::fromStdString(V[i].name));
        textLabel->position->setCoords(x[i], y[i]+0.007);
        plot->addItem(textLabel);
    }

    // replot everything
    plot->replot();
}

void plot_IDsVillages(QCustomPlot *plot,
                      std::vector<stVillage> V)
{
    unsigned int nV= V.size();
    QVector<double> x(nV), y(nV); // initialize with entries 0..100

    for (unsigned int i=0; i< nV; ++i)
    {
        x[i] = V[i].coord.first;
        y[i] = V[i].coord.second;

        QCPItemText *textLabel = new QCPItemText(plot);
        textLabel->setText(QString::number(V[i].ID));
        textLabel->position->setCoords(x[i]+0.007, y[i]);
        plot->addItem(textLabel);
    }

    // replot everything
    plot->replot();
}

void plot_roads(QCustomPlot* plot,
                std::vector<stVillage> V,
                std::vector<stRoad> R,
                std::vector<std::vector<adjmatrix_entry> > dist)
{
    QPointF p1, p2;
    unsigned int nV = V.size();

    for (unsigned int i=0; i< nV; ++i)
    {
        p1.setX(V[i].coord.first);
        p1.setY(V[i].coord.second);

        for (unsigned int j=0; j<nV; ++j)
        {
            if (dist[i][j].roadID!=0)
            {
                p2.setX(V[j].coord.first);
                p2.setY(V[j].coord.second);

                QCPItemLine *arrow = new QCPItemLine(plot);

                switch (R[dist[i][j].roadID-1001].category){
                case 1:arrow->setPen(QPen(Qt::green));break;
                case 2:arrow->setPen(QPen(Qt::yellow));break;
                case 3:arrow->setPen(QPen(Qt::red));break;
                case 4:arrow->setPen(QPen(Qt::black));break;
                }


                plot->addItem(arrow);
                arrow->start->setCoords(p1);
                arrow->end->setCoords(p2);
            }
        }
    }
    // replot everything
    plot->replot();
}

void plot_route_week(QCustomPlot* plot,
                const std::vector<stVillage> V,
                stInterviewer I,
                unsigned int week,// counting starts with 0
                const std::vector<std::vector<unsigned int> > preds)
{

//    unsigned int l = I.routes_weeks[day-1].villages.size();
    std::vector<unsigned int> shortestWay;

    unsigned int next;
    unsigned int pred;

    QVector<double> x, y;

    highlightx.erase(highlightx.begin(), highlightx.end());
    highlighty.erase(highlighty.begin(), highlighty.end());

    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::blue,1));
    myScatter.setBrush(Qt::red);
    myScatter.setSize(5);

    QCPCurve *Curve1 = new QCPCurve(plot->xAxis, plot->yAxis);
    Curve1->setScatterStyle(myScatter);
    Curve1->setPen(QPen(Qt::blue,3));

    pred = I.routes_days[week*5].villages[0];
    highlightx.push_back(V[pred].coord.first);
    highlighty.push_back(V[pred].coord.second);

    for (unsigned int d=0; d<5; ++d)
    {
        for (unsigned int i=1; i< I.routes_days[week*5+d].villages.size(); ++i)
        {
            next = I.routes_days[week*5 + d].villages[i];

            shortestWay = getShortestWay(pred, next, preds[pred]);

            for (unsigned int j = 0; j< shortestWay.size(); ++j)
            {
                next = shortestWay[j];

                x.push_back(V[next].coord.first);
                y.push_back(V[next].coord.second);
            }

            highlightx.push_back(V[next].coord.first);
            highlighty.push_back(V[next].coord.second);

            pred = next;
        }

    }

    // replot everything
    plot->clearPlottables();
    plot->addPlottable(Curve1);
    Curve1->setData(x,y);

    plot->replot();
}

void plot_route_day(QCustomPlot* plot,
                const std::vector<stVillage> V,
                stInterviewer I,
                unsigned int day,
                const std::vector<std::vector<unsigned int> > preds)
{

    unsigned int l = I.routes_days[day-1].villages.size();
    std::vector<unsigned int> shortestWay;

    unsigned int next;
    unsigned int pred;

    highlightx.erase(highlightx.begin(), highlightx.end());
    highlighty.erase(highlighty.begin(), highlighty.end());

    QVector<double> x, y;

    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::blue,1));
    myScatter.setBrush(Qt::red);
    myScatter.setSize(5);

    QCPCurve *Curve1 = new QCPCurve(plot->xAxis, plot->yAxis);
    Curve1->setScatterStyle(myScatter);
    Curve1->setPen(QPen(Qt::blue,3));

    if (l>0)
    {
        pred = I.routes_days[day-1].villages[0];
        highlightx.push_back(V[pred].coord.first);
        highlighty.push_back(V[pred].coord.second);
    }

    for (unsigned int i=1; i< l; ++i)
    {
        next = I.routes_days[day-1].villages[i];

        shortestWay = getShortestWay(pred, next, preds[pred]);

        for (unsigned int j = 0; j< shortestWay.size(); ++j)
        {
            next = shortestWay[j];

            x.push_back(V[next].coord.first);
            y.push_back(V[next].coord.second);
        }

        highlightx.push_back(V[next].coord.first);
        highlighty.push_back(V[next].coord.second);

        pred = next;

    }

    // replot everything
    plot->clearPlottables();

    plot->addPlottable(Curve1);
    Curve1->setData(x,y);

    plot->replot();

}


void delete_labelsAndRoutes(QCustomPlot* plot)
{
    plot->clearItems();
    plot->replot();
}

