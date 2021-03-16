
from django.shortcuts import render

from django.http.response import JsonResponse
from rest_framework.parsers import JSONParser 
from rest_framework import status
 
from coordinates.models import Coordinates
from coordinates.serializers import CoordinateSerializer
from rest_framework.decorators import api_view


@api_view(['GET','POST'])
def handle_coordinates(request):
    if request.method == 'GET':
        coordinate = Coordinates.objects.all()
         
        coordinate_serializer = CoordinateSerializer(coordinate, many=True)
        return JsonResponse(coordinate_serializer.data, safe=False)
    elif request.method == 'POST':
        coordinate_data = JSONParser().parse(request)
        coordinate_serializer = CoordinateSerializer(data=coordinate_data)
        if coordinate_serializer.is_valid():
            coordinate_serializer.save()
            return JsonResponse(coordinate_serializer.data, status=status.HTTP_201_CREATED) 
        return JsonResponse(coordinate_serializer.errors, status=status.HTTP_400_BAD_REQUEST)
 
