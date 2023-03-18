# This file is for automatically find the theif

license_plates = [221103, 686048, 243696, 467400, 398010, 396669, 514354, 560886]
back_accounts = [467400, 395717, 449774, 686048, 458378, 396669, 438727, 514354]
callers = [398010, 560886, 686048, 561160, 560886, 449774, 514354, 907148, 395717, 438727]
passengers = [953679, 398010, 686048, 651714, 560886, 449774, 395717, 467400]
receivers = [864400, 985497, 315221, 652398]

match1 = [id for id in license_plates if id in back_accounts]
match2 = [id for id in match1 if id in callers]
match3 = [id for id in match2 if id in passengers]
print(match3)
