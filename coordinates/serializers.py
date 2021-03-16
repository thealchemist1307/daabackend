from rest_framework import serializers 
from coordinates.models import Coordinates
 
 
class CoordinateSerializer(serializers.ModelSerializer):
 
    class Meta:
        model = Coordinates
        fields = (
                  'x',
                  'y')