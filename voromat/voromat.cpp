#include "voromat.h"
#include "MatlabVoronoiHelper.h"
#include "StatisticsHelper.h"
#include "ColorizeHelper.h"

void voromat::applyFilter(SurfaceMeshModel* mesh, RichParameterSet* pars, StarlabDrawArea* drawArea){
    /// Draw the input vertices if overlay was required
    if(pars->getBool(overlayInput)){
        Vector3VertexProperty points = mesh->get_vertex_property<Vector3>(VPOINT);
        foreach(Vertex v, mesh->vertices())
            drawArea->drawPoint(points[v],1,Qt::red);
    }
            
#ifdef MATLAB
    /// Compute voronoi mapping and measures
    MatlabVoronoiHelper mat(mesh, drawArea);
    mat.createVertexIndexes();
    mat.meshVerticesToVariable("points");
    mat.meshNormalsToVariable("normals");
    mat.computeVoronoiDiagramOf("points");
    mat.searchVoronoiPoles("poleof","scorr");
    mat.getMedialSpokeAngleAndRadii("Vangle","Vradii");
    
    /// Export angle/radii from medial to surface
    mat.eval("vangle=Vangle(poleof);");
    mat.eval("vradii=Vradii(poleof);");   
    mat.variableToVertexScalarProperty("vangle",VANGLE);
    mat.variableToVertexScalarProperty("vradii",VRADII);
    mat.eval("points = loci(poleof,:);");
    
    /// Should we apply the transform?
    string propname = pars->getBool(embedVertices)?VPOINT:VPOLE;
    mat.variableToVector3VertexProperty("points",propname);
#endif

#ifdef QHULL
    TODO THE QHULL VERSION    
#endif
    
    /// Colorize one of the exposed properties
    if( pars->getBool(colorizeRadii) || pars->getBool(colorizeAngle) ){
        mesh->renderer()->setRenderMode("Smooth");
        string propname;
        if( pars->getBool(colorizeRadii) ) propname = VRADII;
        if( pars->getBool(colorizeAngle) ) propname = VANGLE;
        ColorizeHelper(mesh,unsignedColorMap).vscalar_to_vcolor(propname);
        // qDebug() << ScalarStatisticsHelper(mesh).statistics(propname);
    }
}

Q_EXPORT_PLUGIN(voromat)
