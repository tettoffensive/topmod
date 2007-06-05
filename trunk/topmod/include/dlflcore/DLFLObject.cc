/**
 * \file DLFLObject.cc
 */

#include "DLFLObject.hh"

namespace DLFL {

  uint DLFLObject::suLastID = 0;
  Transformation DLFLObject::tr;

  void DLFLObject::dump(ostream& o) const {
    o << "DLFLObject" << endl;

    int i=0;

    o << "VertexList" << endl;
  
    DLFLVertexPtrList::const_iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
  
    while ( vfirst != vlast ) {
      o << i << " : " << (*vfirst) << endl;
      (*vfirst)->dump(o);
      ++vfirst; ++i;
    }

    i = 0;
    o << "EdgeList" << endl;
    DLFLEdgePtrList::const_iterator efirst=edge_list.begin(), elast=edge_list.end();
    while ( efirst != elast ) {
      o << i << " : " << (*efirst) << endl;
      (*efirst)->dump(o);
      ++efirst; ++i;
    }
         
    i = 0;
    o << "FaceList" << endl;
    DLFLFacePtrList::const_iterator ffirst=face_list.begin(), flast=face_list.end();
    while ( ffirst != flast ) {
      o << i << " : " << (*ffirst) << endl;
      (*ffirst)->dump(o);
      ++ffirst; ++i;
    }
  }

  void DLFLObject::splice(DLFLObject& object) {
    // Combine 2 objects. The lists are simply spliced together.
    // Entities must be removed from the second object to prevent dangling pointers
    // when it is destroyed.
    vertex_list.splice(vertex_list.end(),object.vertex_list);
    edge_list.splice(edge_list.end(),object.edge_list);
    face_list.splice(face_list.end(),object.face_list);
    matl_list.splice(matl_list.end(),object.matl_list);
  }

  // Reverse the orientation of all faces in the object
  // This also requires reversing all edges in the object
  void DLFLObject::reverse(void)
  {
    // Reverse the edges first, since they depend on the ordering of the
    // original faces.
    DLFLEdgePtrList::iterator efirst=edge_list.begin(), elast=edge_list.end();
    while ( efirst != elast ) {
      (*efirst)->reverse();
      ++efirst;
    }
  
    DLFLFacePtrList::iterator ffirst=face_list.begin(), flast=face_list.end();
    while ( ffirst != flast ) {
      (*ffirst)->reverse();
      ++ffirst;
    }
  }
  /*
    void DLFLObject::edgeCleanup(void) {
    edgeCleanup(edge_list);
    }

    void DLFLObject::edgeCleanup(const DLFLEdgePtrList& edges) {
    DLFLEdgePtrList deletion_list; // List of edges to be deleted
    DLFLEdgePtrList::const_iterator first, last;
    DLFLEdgePtr edge;
    DLFLFacePtr fp1, fp2;

    // First go through all edges and find out ones which have to be deleted.
    // If we do deletion on the fly, some normal computation could be affected
    // causing errors
    first = edges.begin(); last = edges.end();
    while ( first != last ) {
    edge = (*first); ++first;
    edge->getFacePointers(fp1,fp2);

    if ( fp1 != fp2 )
    {
    Vector3d fp1n = fp1->computeNormal();
    Vector3d fp2n = fp2->computeNormal();
    if ( Abs(fp1n*fp2n-1.0) < 1.0e-5 )
    {
    // Normals of faces on both sides of edge are same
    // This edge can be deleted
    deletion_list.push_back(edge);
    }
    }
    else
    deletion_list.push_back(edge);
    }

    first = deletion_list.begin(); last = deletion_list.end();
    while ( first != last ) {
    edge = (*first); ++first;
    deleteEdge(edge,true);
    }
    }

    void DLFLObject::edgeCleanup(const DLFLEdgePtrArray& edges) {
    DLFLEdgePtrList deletion_list; // List of edges to be deleted
    DLFLEdgePtrList::iterator first, last;
    DLFLEdgePtr edge;
    DLFLFacePtr fp1, fp2;

    // First go through all edges and find out ones which have to be deleted.
    // If we do deletion on the fly, some normal computation could be affected
    // causing errors
    for (int i=0; i < edges.size(); ++i) {
    edge = edges[i];
    edge->getFacePointers(fp1,fp2);

    if ( fp1 != fp2 )
    {
    Vector3d fp1n = fp1->computeNormal();
    Vector3d fp2n = fp2->computeNormal();
    if ( Abs(fp1n*fp2n-1.0) < 1.0e-5 )
    {
    // Normals of faces on both sides of edge are same
    // This edge can be deleted
    deletion_list.push_back(edge);
    }
    }
    else
    deletion_list.push_back(edge);
    }

    first = deletion_list.begin(); last = deletion_list.end();
    while ( first != last ) {
    edge = (*first); ++first;
    deleteEdge(edge,true);
    }
    }

    // Removes one of the edges in 2-gons
    void DLFLObject::cleanup2gons(void) {
    // Go through list of faces. If a 2-gon is found,
    // delete one of the edges.
    // We don't check for infinite loop, since if an edge belongs
    // to a 2-gon deleting it will not create a new face
    DLFLFacePtrList::iterator ffirst=face_list.begin(), flast=face_list.end();
    DLFLFacePtr fp;
    while ( ffirst != flast ) {
    fp = (*ffirst); ++ffirst;
    if ( fp->size() == 2 )
    {
    // Face is a 2-gon.
    // Get one of the edges in the face and delete it
    DLFLFaceVertexPtr fvp = fp->front();
    DLFLEdgePtr ep = fvp->getEdgePtr();
    deleteEdge(ep,true);
    }
    }
    }


    // Removes all winged (valence-2) vertices
    void DLFLObject::cleanupWingedVertices(void) {
    // Go through list of vertices. If a valence-2 vertex is found,
    // find the 2 edges incident on that vertex. Insert an edge
    // between the other vertices of the 2 edges belonging to
    // the same face. Delete the edges incident on the valence-2 vertex
    // with cleanup to get rid of the point sphere.
    DLFLVertexPtrList::iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
    DLFLVertexPtr vp;
    DLFLFaceVertexPtrArray fvparray;
    DLFLEdgePtrArray eparray;
    DLFLFaceVertexPtr fvp, pfvp, nfvp;
    while ( vfirst != vlast ) {
    vp = (*vfirst); ++vfirst;
    // If vp is a valence 2 vertex, it will be removed, so increment the iterator now itself
    if ( vp->valence() == 2 )
    {
    vp->getFaceVertices(fvparray);
    vp->getEdges(eparray);

    // Pick the first face vertex in the array
    fvp = fvparray[0];

    // Find the previous and next corners to fvp
    pfvp = fvp->prev(); nfvp = fvp->next();

    if ( pfvp == nfvp )
    {
    // We have a disconnected 2-gon (consisting of 2 faces, 2 vertices and 2 edges)
    // This component will be left as is. Deleting this component is tricky, since
    // we are traversing the vertex list and can't remove vertices arbitrarily
    // We can keep track of such cases and handle them outside the loop
    }
    else
    {
    // Insert an edge between pfvp and nfvp
    // We know that both belong to the same face
    insertEdge1(pfvp,nfvp);

    // Now delete the 2 edges incident on the valence-2 vertex with cleanup
    deleteEdge(eparray[0],true);
    deleteEdge(eparray[1],true);
    }

    }
    }
    }
  */
  bool DLFLObject::edgeExists(DLFLVertexPtr vptr1, DLFLVertexPtr vptr2) {
    // Check if an edge exists between two given vertices
    bool edgeexists = false;

    // Try to find an edge from vptr1 to vptr2
    if ( vptr1->getEdgeTo(vptr2) != NULL ) edgeexists = true;

    return edgeexists;
  }
  /**/

  void DLFLObject::createFace(const Vector3dArray& verts, DLFLMaterialPtr matl, bool set_type) {
    // Create 2 new faces with the given vertex coordinates. The 2 faces will have the same
    // vertices and share the same edges, but will have opposite rotation orders.
    // This essentially creates a 2 manifold with 2 faces with no volume.
    int numverts = verts.size();
    DLFLFacePtr newface1, newface2;
    DLFLVertexPtr vptr, tempvptr;
    DLFLFaceVertex fv;
    DLFLFaceVertexPtr fvptr;

    if ( matl == NULL ) matl = firstMaterial();
  
    newface1 = new DLFLFace; newface2 = new DLFLFace;

    if ( set_type ) {
      newface1->setType(FTNew); newface2->setType(FTNew);
    }
  
    for (int i=0; i < numverts; ++i) {
      vptr = new DLFLVertex(verts[i]); if ( i == 0 ) tempvptr = vptr;
      addVertexPtr(vptr);
      fv.setVertexPtr(vptr);
      newface1->addVertex(fv); newface2->addVertex(fv);
    }

    // Reverse one of the new faces
    newface2->reverse();

    // These updates have to be done before the reorder
    newface1->addFaceVerticesToVertices(); newface2->addFaceVerticesToVertices();

    // Reorder the second new face so that the first vertex is the first vertex in the array
    DLFLFaceVertexPtrList fvplist = tempvptr->getFaceVertexList();
    fvptr = fvplist.front();
    if ( fvptr->getFacePtr() != newface2 ) fvptr = fvplist.back();
    newface2->reorder(fvptr);
  
    // Add the new faces to the list
    addFacePtr(newface1); addFacePtr(newface2);
    //newface1->setMaterial(matl); newface2->setMaterial(matl);

    newface1->computeNormal(); newface2->computeNormal();

    // Create the new edges
    // Traverse the 2 faces in opposite orders
    DLFLFaceVertexPtr head1, current1;
    DLFLFaceVertexPtr head2, current2;
    DLFLEdgePtr eptr;

    head1 = newface1->front(); head2 = newface2->back();

    // Assume that head1 and head2 are not NULL
    current1 = head1; current2 = head2;
    eptr = new DLFLEdge(current1,current2);
    eptr->updateFaceVertices();
    addEdgePtr(eptr);
    current1 = current1->next(); current2 = current2->prev();
    while ( current1 != head1 && current2 != head2 ) {
      eptr = new DLFLEdge(current1,current2);
      eptr->updateFaceVertices();
      addEdgePtr(eptr);
      current1 = current1->next(); current2 = current2->prev();
    }
  }

  DLFLFaceVertexPtr DLFLObject::createPointSphere(const Vector3d& v, DLFLMaterialPtr matl) {
    // Create a point sphere - a face with only 1 vertex
    DLFLFacePtr newface = new DLFLFace();

    if ( matl == NULL ) matl = firstMaterial();

    DLFLVertexPtr vp = new DLFLVertex(v);
    DLFLFaceVertexPtr fvp = new DLFLFaceVertex(vp,NULL);
    fvp->addSelfToVertex();
    newface->addVertexPtr(fvp);
    //newface->setMaterial(matl);

    addVertexPtr(vp);
    addFacePtr(newface);

    return fvp;
  }

  void DLFLObject::boundingBox(Vector3d& min, Vector3d& max) const {
    DLFLVertexPtrList::const_iterator vf,vl;
    double minx,miny,minz;
    double maxx,maxy,maxz;
    double x,y,z;
    DLFLVertexPtr vptr;
  
    vf = vertex_list.begin(); vl = vertex_list.end();
    vptr = (*vf); ++vf;

    vptr->coords.get(minx,miny,minz);
    maxx = minx; maxy = miny; maxz = minz;
  
    while ( vf != vl ) {
      vptr = (*vf); ++vf;
      vptr->coords.get(x,y,z);

      (x < minx) ? minx = x : ( (x > maxx) ? maxx = x : 1);
      (y < miny) ? miny = y : ( (y > maxy) ? maxy = y : 1);
      (z < minz) ? minz = z : ( (z > maxz) ? maxz = z : 1);
    }
    min.set(minx,miny,minz); max.set(maxx,maxy,maxz);
  }

  bool DLFLObject::boundaryWalkID( uint faceId ) {
    DLFLFacePtr fp = findFace( faceId );
    if( fp ) { 
      fp->boundaryWalk();
      return true;
    } else { return false; }
  }

  vector<int> DLFLObject::vertWalk( uint faceId ) { 
    DLFLFacePtr fp = findFace( faceId );
    vector<int> verts;
    if( fp ) { 
      verts = fp->vertexWalk();
    }
    return verts;
  }

  vector<int> DLFLObject::edgeWalk( uint faceId ) {
    DLFLFacePtr fp = findFace( faceId );
    vector<int> edges;
    if( fp ) { 
      edges = fp->edgeWalk();
    } 
    return edges;
  }

  void DLFLObject::boundaryWalk(uint face_index) {
    //Find the Face with the given face_index from the FaceList and do a boundary walk on it
    uint i = 0;
    DLFLFacePtrList::iterator first = face_list.begin();
    DLFLFacePtrList::iterator last = face_list.end();
    DLFLFacePtr faceptr = NULL;

    if (face_index > face_list.size())
      return;
    while (first != last) {
      if (i == face_index) {
	faceptr = (*first);
	break;
      }
      ++first;

      ++i;
    }
    faceptr->boundaryWalk();
  }

  void DLFLObject::vertexTrace(uint vertex_index) {
    //Find the Vertex with the given index in the VertexList and do a vertex trace on it
    uint i = 0;
    DLFLVertexPtrList::iterator first = vertex_list.begin();
    DLFLVertexPtrList::iterator last = vertex_list.end();
    DLFLVertexPtr vertexptr = NULL;

    if (vertex_index > vertex_list.size())
      return;
    while (first != last) {
      if (i == vertex_index) {
	vertexptr = (*first);
	break;
      }
      ++first;

      ++i;
    }
    vertexptr->vertexTrace();
  }

  DLFLVertexPtr DLFLObject::findVertex(const uint vid)
  {
    // Find a vertex with the given vertex id. Return NULL if none exists
    DLFLVertexPtrList::iterator first = vertex_list.begin(), last = vertex_list.end();
    DLFLVertexPtr sel = NULL;
    while ( first != last )
      {
	if ( (*first)->getID() == vid )
          {
            sel = (*first); break;
          }
	++first;
      }
    return sel;
  }

  DLFLEdgePtr DLFLObject::findEdge(const uint eid) {
    // Find an edge with the given edge id. Return NULL if none exists
    DLFLEdgePtrList::iterator first = edge_list.begin(), last = edge_list.end();
    DLFLEdgePtr sel = NULL;
    while ( first != last ) {
      if( (*first)->getID() == eid ) {
	sel = (*first); 
	break;
      }
      ++first;
    }
    return sel;
  }
  
  DLFLFacePtr DLFLObject::findFace(const uint fid) {
    // Find a face with the given face id. Return NULL if none exists
    DLFLFacePtrList::iterator first = face_list.begin(), last = face_list.end();
    DLFLFacePtr sel = NULL;
    while ( first != last ) {
	if ( (*first)->getID() == fid ) {
	  sel = (*first); 
	  break;
	}
	++first;
    }
    return sel;
  }

  void DLFLObject::addVertex(const DLFLVertex& vertex)
  {
    addVertexPtr(vertex.copy());
  }
     
  void DLFLObject::addVertex(DLFLVertexPtr vertexptr)
  {
    addVertexPtr(vertexptr->copy());
  }
     
  void DLFLObject::addEdge(const DLFLEdge& edge)
  {
    addEdgePtr(edge.copy());
  }
     
  void DLFLObject::addEdge(DLFLEdgePtr edgeptr)
  {
    addEdgePtr(edgeptr->copy());
  }
     
  void DLFLObject::addFace(const DLFLFace& face)
  {
    addFacePtr(face.copy());
  }

  void DLFLObject::addFace(DLFLFacePtr faceptr)
  {
    addFacePtr(faceptr->copy());
  }

  void DLFLObject::computeNormals( ) {
    DLFLVertexPtrList::iterator first, last;

    first = vertex_list.begin(); last = vertex_list.end();
    while ( first != last ) {
      (*first)->updateNormal();
      ++first;
    }
  }

  /*
    void DLFLObject::deleteVertex(DLFLVertexPtr vertexptr)
    {
    // A Vertex can be deleted only if it is a winged vertex - has only 2 edges incident on it
    // Deleting the vertex basically connects the 2 edges on either side, which involves, deleting
    // those 2 edges and adding a new edge.
    int valence = vertexptr->numEdges();

    if ( valence != 2 ) return;

    // Not yet implemented
    }

    void DLFLObject::deleteVertex(uint vertex_index)
    {
    // Find the VertexPtr for the given vertex_index from the VertexList and delete it
    if ( vertex_index > vertex_list.size() ) return;

    DLFLVertexPtrList::iterator i = vertex_list.begin();
    advance(i,vertex_index);
    deleteVertex(*i);
    }*/

} // end namespace