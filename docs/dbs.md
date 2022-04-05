# Database scheme:
> 
<img width="677" alt="image" src="https://user-images.githubusercontent.com/43150028/161834925-9df2d5f6-78e5-4cfe-a3e4-a04db76a9e43.png">


### `Helicopters` instance:
 | Field              | Type        |
 | -----------        | ----------- |
 | id                 | integer P   |
 | brand              | text        |
 | production_date    | date        |
 | load_capacity      | integer     |
 | last_overhaul_date | date        |
 | flight_resource    | integer     |


### `Pilots` instance:
 | Field         | Type        |
 | ------------- | ----------- |
 | id            | integer P   |
 | surname       | text        |
 | position_id   | integer FK  |
 | experience    | integer     |
 | address       | text        |
 | birth_year    | integer     |
 | helicopter_id | integer FK  |
 | login         | text        |
 | password      | text        |


### `Positions` instance:
 | Field         | Type        |
 | ------------- | ----------- |
 | id            | integer PK  |
 | name          | text        |


### `Sums` instance:
 | Field         | Type        |
 | ------------- | ----------- |
 | id            | integer PK  |
 | period_start  | date        |
 | period_end    | date        |
 | sums          | integer     |


### `Flights` instance:
 | Field         | Type        |
 | ------------- | ----------- |
 | id            | integer PK  |
 | helicopter_id | integer FK  |
 | date          | date        |
 | mass_cargo    | integer     |
 | people_amount | integer     |
 | duration      | integer     |
 | price         | integer     |
 | type_id       | integer FK  |


### `Types` instance:
 | Field         | Type        |
 | ------------- | ----------- |
 | id            | integer PK  |
 | name          | text        |
 | salary_ratio  | real        |