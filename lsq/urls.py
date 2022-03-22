from django.conf.urls import url 
from lsq import views 
 
urlpatterns = [ 
    url(r'^api/lsq$', views.handle_coordinates)]
    