#include <hrd/plot.h>

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
    plot->clearItems();
    plot->clearGraphs();
    plot->replot();

    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(QBrush(Qt::white));
    myScatter.setSize(5);

    // create graph and assign data to it:
    plot->addGraph();
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

    // extra mark capital
    xNouna[0] = V[142-101].coord.first;
    yNouna[0] = V[142-101].coord.second;

    plot->addGraph();
    myScatter.setBrush(QBrush(Qt::red));
    myScatter.setSize(7);
    plot->graph(1)->setScatterStyle(myScatter);
    plot->graph(1)->setData(xNouna, yNouna);

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
        textLabel->position->setCoords(x[i], y[i]+0.007);
        plot->addItem(textLabel);
    }

    // replot everything
    plot->replot();
}

void plot_roads(QCustomPlot *plot,
                std::vector<stVillage> V,
                std::vector<stRoad> R,
                std::vector<std::vector<uDist> > dist,
                bool rainingSeason)
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

void plot_route(QCustomPlot *plot,
                const std::vector<stVillage>& V,
                stInterviewer I,
                unsigned int day,
                const std::vector<std::vector<unsigned int> >& preds)
{

    QPointF p1, p2;

    unsigned int l = I.routes[day-1].villages.size();
    std::cout << "Length of the route " << l << std::endl;
    std::vector<unsigned int> shortestWay;

    unsigned int next;
    unsigned int pred;

    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::red);
    myScatter.setSize(5);

    pred = I.routes[day-1].villages[0];
    p1.setX(V[pred].coord.first);
    p1.setY(V[pred].coord.second);

    for (unsigned int i=1; i< l; ++i)
    {
        next = I.routes[day-1].villages[i];
        std::cout << "next " << next;

        shortestWay = getShortestWay(pred, next, preds[pred]);

//        p2.setX(V[next].coord.first);
//        p2.setY(V[next].coord.second);

//        QCPItemLine *arrow = new QCPItemLine(plot);
//        arrow->setPen(QPen(Qt::black));
//        plot->addItem(arrow);
//        arrow->start->setCoords(p1);
//        arrow->end->setCoords(p2);

//        pred = next;
//        p1 = p2;

        std::cout << "Shortest way:";
        for (unsigned int j = 0; j< shortestWay.size(); ++j)
        {
            std::cout << shortestWay[j] << " ";
        }
        std::cout << std::endl;

        for (unsigned int j = 0; j< shortestWay.size(); ++j)
        {
            next = shortestWay[j];

            p2.setX(V[next].coord.first);
            p2.setY(V[next].coord.second);

            QCPItemLine *arrow = new QCPItemLine(plot);
            arrow->setPen(QPen(Qt::black));
            plot->addItem(arrow);
            arrow->start->setCoords(p1);
            arrow->end->setCoords(p2);
            if (j == shortestWay.size()-1)
                arrow->setHead(QCPLineEnding::esSpikeArrow);

            p1 = p2;
        }

        pred = next;

    }
    // replot everything
    plot->replot();
    std::cout << "finished plotting route" << std::endl;
}

void plot_routes(QCustomPlot *plot,
                 std::vector<stVillage> V,
                 stInterviewer I)
{
    plot->clearItems();
    plot->replot();

    QPointF p1, p2;

    for (unsigned int d=0; d<I.routes.size(); ++d)
    {
        unsigned int l = I.routes[d].villages.size();
        std::cout << "Length of the route " << l << std::endl;

        unsigned int next;
        unsigned int pred = I.routes[d].villages[0];

        p1.setX(V[pred].coord.first);
        p1.setY(V[pred].coord.second);

        for (unsigned int i=1; i< l; ++i)
        {
            next = I.routes[d].villages[i];

            p2.setX(V[next].coord.first);
            p2.setY(V[next].coord.second);

            QCPItemLine *arrow = new QCPItemLine(plot);
            arrow->setPen(QPen(Qt::black));
            plot->addItem(arrow);
            arrow->start->setCoords(p1);
            arrow->end->setCoords(p2);

            pred = next;
            p1 = p2;
        }
    }
    // replot everything
    plot->replot();
}

void delete_labelsAndRoutes(QCustomPlot* plot)
{
    plot->clearItems();
    plot->replot();
}

