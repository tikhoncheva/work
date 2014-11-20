#include <hrd/plot.h>

void plot_villages(QCustomPlot *plot,
                   std::vector<stVillage> V)
{
    plot->clearItems();
    plot->clearGraphs();
    plot->replot();

    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::white);
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

void plot_routes(QCustomPlot *plot,
                 std::vector<stVillage> V,
                 std::vector<stRoute> R,
                 std::vector<std::vector<unsigned int> > dist,
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
            if (dist[i][j]!=0)
            {
                p2.setX(V[j].coord.first);
                p2.setY(V[j].coord.second);

                QCPItemLine *arrow = new QCPItemLine(plot);

                switch (R[dist[i][j]-1001].category){
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

void delete_labelsAndRoutes(QCustomPlot* plot)
{
    plot->clearItems();
    plot->replot();
}

