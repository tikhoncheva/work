N=4; knn_size = 2;
%%
x=rand(N,2); M=N*N-N; s=zeros(M,3); j=1;
for i=1:N
  for k=[1:i-1,i+1:N]
    s(j,1)=k; s(j,2)=i; s(j,3)=exp(-sum((x(i,:)-x(k,:)).^2));
    j=j+1;
  end;
end;
p=median(s(:,3));

%% neares neighbors of each node
S = reshape([s(:,3);p*ones(N,1)], N, N);
S = S.*double(~eye(N));
% S = S + diag(-Inf*ones(N,1));
S = S + diag(ones(N,1));
Best = {};
parfor i = 1:N
    [V, I] = sort(S(i,:), 'descend');
    Best{i} = [I(1:knn_size)'];
end
knn = (cell2mat(Best))';

% create knn-graph
list_knn = (knn-ones(N,knn_size))*N+repmat( (1:N)',1,knn_size );
knn_mask = false(N); knn_mask(list_knn) = true;

%%

% p = diag(S);
p=median(s(:,3));

% [idx,netsim,dpsim,expref] = apclusterSparse_init(s,p,'plot');
% [idx,netsim,dpsim,expref] = apclusterSparse(s,p,'plot');
[idx,netsim,dpsim,expref] = apclusterSparse_kNN(s,p,knn_mask,'plot','convits', 100);
% [idx,netsim,dpsim,expref] = apclusterSparse_sumprod(s,p,'plot');

fprintf('Number of clusters: %d\n',length(unique(idx)));
fprintf('Fitness (net similarity): %g\n',netsim);
figure;
for i=unique(idx)'
  ii=find(idx==i); h=plot(x(ii,1),x(ii,2),'o'); hold on;
  col=rand(1,3); set(h,'Color',col,'MarkerFaceColor',col);
  xi1=x(i,1)*ones(size(ii)); xi2=x(i,2)*ones(size(ii)); 
  line([x(ii,1),xi1]',[x(ii,2),xi2]','Color',col);
end;
axis equal tight;