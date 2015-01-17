from django.db import models

# Create your models here.

class UserProfile(models.Model):
	user = models.OneToOneField(User)
	context_io_id = models.CharField(max_length=24)
