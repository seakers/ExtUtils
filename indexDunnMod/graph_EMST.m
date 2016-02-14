function [G,dist,connect] = graph_EMST(data,options)
% GRAPH_EMST finds the Euclidean Minimum Spanning Tree graph among points
% in P dimensions. 
% Usage: [G,dist,connect] = graph_EMST(data,options)
% -------------------------------------------------------------------------
% INPUTS:
% data          : [n x p] matrix of point coordinates.
% options.show  : if 0, plot of the graph is not shown [default=0]
% -------------------------------------------------------------------------
% OUTPUTS:
% G             : [n x n] sparse matrix that represents a graph. Nonzero
%                 entries in matrix G represent the weights of the edges.
% dist          : corresponding edge lengths (Euclidean distances);
%                 non-connected edge distances are given as zero.
% connect       : [n x n] boolean adjacency matrix.
% -------------------------------------------------------------------------
% Implementation inspired by:
% http://en.wikipedia.org/wiki/Euclidean_minimum_spanning_tree
%--------------------------------------------------------------------------
% Copyright (C) 2013,  Nejc Ilc
%--------------------------------------------------------------------------

if nargin < 2
	options.show=[];
end

if isempty(options)
	options.show=0;
end
[N,D]=size(data);

% compute Delaunay triangulation

% 2-D and 3-D case
if D < 4
	DT=DelaunayTri(data);
else
	DT=delaunayn(data);
end

E = edges(DT);
nEdges=size(E,1);
dist = dist_euclidean(data,data);

%create sparse matrix - graph representation
C=sparse(E(:,1),E(:,2),ones(nEdges,1),N,N,nEdges);
connect=C;
G= C+C';
% weight graph by distances
G=G.*dist;
dist=sqrt(dist); % leave G without sqrt for now for numeric accuracy. Equivalent for Kruskal.

% graph is undirected, so we can forget half of edges
% G=tril(G); %removed when running kruskal instead of graphminspantree

% compute minimal spanning tree on graph
% [G] = graphminspantree(G,'method','kruskal'); % bioinformatics toolbox
[~,MSTedges,~]=kruskal(G>0,G);
G=sparse(MSTedges(:,1),MSTedges(:,2),G(sub2ind(size(G),MSTedges(:,1),MSTedges(:,2))),N,N);
G=sqrt(G); % correct by sqrting G.

if (options.show==1)
	fig=figure();
	gplot(G,data,'-k');
	hold on;
	plot(data(:,1),data(:,2),'.r');
	axis('equal');
end

