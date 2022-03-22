
from django.conf.urls import url 
from sweep import views 



urlpatterns = [ 
    url(r'^api/sweep$', views.handle_coordinates_sweep)]
    