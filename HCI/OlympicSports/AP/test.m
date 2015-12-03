N=100; x=rand(N,2); M=N*N-N; s=zeros(M,3); j=1;
for i=1:N
  for k=[1:i-1,i+1:N]
    s(j,1)=k; s(j,2)=i; s(j,3)=-sum((x(i,:)-x(k,:)).^2);
    j=j+1;
  end;
end;
p=median(s(:,3));

%% neares neighbors of each node
knn_size = 50;
S = reshape([s(:,3);p*ones(N,1)], N, N);
Best = {};
parfor i = 1:N
    [V, I] = sort(S(i,:), 'descend');
    Best{i} = [I(1:knn_size)'];
end

list_knn = (cell2mat(Best))';

%%

% [idx,netsim,dpsim,expref] = apclusterSparse_init(s,p,'plot');
% [idx,netsim,dpsim,expref] = apclusterSparse(s,p,'plot');
[idx,netsim,dpsim,expref] = apclusterSparse_kNN(s,p,list_knn,'plot');
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